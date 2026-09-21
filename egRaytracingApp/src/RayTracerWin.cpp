// RayTracerWin.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "RayTracerWin.h"
#include "MathHelpers.h"
#include <hc/assets/hcAssetManager.h>

// include directx math for vector and matrix operations
#include  <DirectXMath.h>

using namespace DirectX;

#define MAX_LOADSTRING 100

constexpr float PI = 3.14159265358979323846f;
constexpr float EPSILON = 0.0001f;

float Saturate(float value)
{
  return std::max(0.0f, std::min(1.0f, value));
}

/**
 * GGX
 */
float DistributionGGX(const Vector3f& N, const Vector3f& H, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = std::max(N.dot(H), 0.0f);
  float NdotH2 = NdotH * NdotH;
  float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
  denom = PI * denom * denom;
  return a2 / std::max(denom, EPSILON);
}

/**
 * Schlick-GGX
 */
float GeometrySchlickGGX(float NdotX, float roughness)
{
  const float r = roughness + 1.0f;
  const float k = (r * r) / 8.0f;
  return NdotX / std::max((NdotX * (1.0f - k) + k), EPSILON);
}

/**
 * Smith's method for geometry term (isotropic)
 */
float GeometrySmith(const Vector3f& N, const Vector3f& V, const Vector3f& L, float roughness)
{
  float NdotV = std::max(N.dot(V), 0.0f);
  float NdotL = std::max(N.dot(L), 0.0f);
  float ggx1 = GeometrySchlickGGX(NdotV, roughness);
  float ggx2 = GeometrySchlickGGX(NdotL, roughness);
  return ggx1 * ggx2;
}

/**
 * Fresnel-Schlick approximation
 */
Color FresnelSchlick(float cosTheta, const Color& F0)
{
  const float fresnelFactor = std::pow(1.0f - Saturate(cosTheta), 5.0f);
  return F0 + (Color::White() - F0) * fresnelFactor;
}

Color EvaluatePBR(
  const Vector3f& N,
  const Vector3f& V, // remember V points towards the camera
  const Vector3f& L, // remember L points towards the light
  const Material& material
)
{
  const float metallic = Saturate(material.metallic);
  const float roughness = std::max(Saturate(material.roughness), 0.04f); // Clamp roughness to avoid division by zero)

  const Vector3f H = (V + L).normalized();

  const float NdotL = std::max(N.dot(L), 0.0f); // light area
  const float NdotV = std::max(N.dot(V), 0.0f); // light area that I can see
  const float NdotH = std::max(N.dot(H), 0.0f); // specular area
  const float VdotH = std::max(V.dot(H), 0.0f); // specular area that I can see

  if (NdotL <= 0.0f || NdotV <= 0.0f)
    return Color::Black();

  const float F0_dielectric = std::pow((material.ior - 1.0f) / (material.ior + 1.0f), 2.0f); // Fresnel reflectance at normal incidence for dielectrics

  // The SPECULAR color when the view is at grazing angles. For dielectrics, this depends
  // on the F0_dielectric, while for metals, it's the albedo color.
  const Color F0 = Color::Lerp(Color(F0_dielectric, F0_dielectric, F0_dielectric), material.albedo, metallic);

  const float D = DistributionGGX(N, H, roughness);
  const float G = GeometrySmith(N, V, L, roughness);
  const Color F = FresnelSchlick(VdotH, F0);

  const float denominator = 4.0f * std::max(NdotV, EPSILON) * std::max(NdotL, EPSILON);
  const Color specular = (D * G * F) / std::max(denominator, EPSILON);

  // (kD * albedo) / PI
  // We need KS, but to calculate it we need to know the Fresnel term F, which is already calculated above.
  // To calculate the kD we use the formula kD = 1 - F. In other words, it is what is not reflected, but absorbed by the material.
  Color kD = Color::White() - F;
  kD *= (1.0f - metallic); // Metals have no diffuse component, so we multiply by (1 - metallic)

  const Color diffuse = (kD * material.albedo);
  return diffuse + specular;
}

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

Vector<Material> g_materials;       // Vector to hold the materials in the scene
Vector<Sphere> g_sceneSpheres;      // Vector to hold the spheres in the scene
Vector<Plane> g_scenePlanes;        // Vector to hold the planes in the scene
Vector<Triangle> g_sceneTriangles;  // Vector to hold the triangles in the scene
hc::AssetManager g_assetManager;    // Asset manager for loading assets
hc::UniquePtr<Octree<Triangle>> g_sceneOctree; // Octree for spatial partitioning of triangles

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// RAYTRACING 
HitInfo FindClosestHit(const Ray& ray, REAL_TYPE minT, REAL_TYPE maxT);
Vector4f FindColorForRay(const Ray& ray, REAL_TYPE minT, REAL_TYPE maxT, Int32 maxBounces);
bool IntersectRaySphere(const Ray& ray, const Sphere& sphere, REAL_TYPE minT, REAL_TYPE maxT, HitInfo& hitInfo);
bool IntersectRayPlane(const Ray& ray, const Plane& plane, REAL_TYPE minT, REAL_TYPE maxT, HitInfo& hitInfo);
bool IntersectRayTriangle(const Ray& ray, const Triangle& triangle, REAL_TYPE minT, REAL_TYPE maxT, HitInfo& hitInfo);
bool IsInShadow(const HitInfo& hit, const Vector3f& lightPosition);
void RaytraceScreen(HDC hdc, int width, int height);
void GetTrianglesFromModel(const hc::Model& model, Vector<Triangle>& outVertices);
AABB ComputeSceneAABB(const Vector<Triangle>& sceneTriangles);

/////////////////////////////////////////////////////////////////////////////////////////
// SCENE PROPERTIES

const UInt32 g_antialiasingSamples = 1;
const float g_cameraRotation = 30.0f;
//const String g_modelPath = "spunky/Spunky.obj";
const String g_modelPath = "spaceships-scene/spaceships-scene.objs"; // I misspelled the extesion to not use meshes for now.
/////////////////////////////////////////////////////////////////////////////////////////

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR    lpCmdLine,
  _In_ int       nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_RAYTRACERWIN, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAYTRACERWIN));

  MSG msg;

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYTRACERWIN));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RAYTRACERWIN);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance; // Store instance handle in our global variable

  int windowWidth = 800;
  int windowHeight = 600;

  HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd)
  {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
  srand(GetTickCount64());

  g_materials.resize(3);
  Material redMaterial = g_materials[0];
  //redMaterial.albedo = Color::RandomHSV(1.0f, 0.0f);
  redMaterial.albedo = Color::Gold();
  redMaterial.metallic = 0.5f;
  redMaterial.roughness = 0.4f;

  Material tealMaterial = g_materials[1];
  tealMaterial.albedo = Color::RandomHSV(1.0f, 0.0f);
  tealMaterial.metallic = 0.3f;
  tealMaterial.roughness = 0.5f;

  Material floorMaterial = g_materials[2];
  floorMaterial.albedo = Color::RandomHSV(1.0f, 0.0f);
  floorMaterial.metallic = 0.1f;
  floorMaterial.roughness = 0.5f;

  g_sceneSpheres.push_back(
    Sphere(
      Vector3f(3.0f, 0.0f, 0.0f),
      1.0f,
      redMaterial
    )
  );

  g_sceneSpheres.push_back(
    Sphere(
      Vector3f(3.0f, 5.0f, 0.0f),
      1.0f,
      tealMaterial
    )
  );

  g_sceneSpheres.push_back(
    Sphere(
      Vector3f(-3.0f, 0.0f, 0.0f),
      1.0f,
      tealMaterial
    )
  );

  /*
  g_sceneSpheres.push_back(
    Sphere(
      Vector3f(3.0f, 3.0f, 3.0f),
      1.0f,
      Color::Blue(),
      0.3,
      0.8,
      0.2
    )
  );*/

  g_scenePlanes.push_back(
    Plane(
      Vector3f(0.0f, -1.0f, 0.0f),
      Vector3f(0.0f, 1.0f, 0.0f),
      floorMaterial
    )
  );

  // prepares hot-coffee's asset manager
  hc::Path rootPath = hc::Path::GetCurrentPath() / "../../egRaytracingApp/assets";
  g_assetManager.setRootPath(rootPath);

  // loads Model and get triangles
  hc::SharedPtr<hc::Model> spunky = g_assetManager.getModelAssetManager().load(rootPath / g_modelPath);
  if (spunky != nullptr)
    GetTrianglesFromModel(*spunky, g_sceneTriangles);

  // creates scene's octree
  AABB sceneAABB = ComputeSceneAABB(g_sceneTriangles);

  g_sceneOctree = hc::MakeUnique<Octree<Triangle>>(sceneAABB);
  for (auto& triangle : g_sceneTriangles)
    g_sceneOctree->insert(&triangle, triangle.aabb);
  
  return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // Parse the menu selections:
    switch (wmId)
    {

    case ID_RENDER_RENDER:
    {
      RECT clientRect;
      GetClientRect(hWnd, &clientRect);
      int width = clientRect.right - clientRect.left;
      int height = clientRect.bottom - clientRect.top;
      HDC hdc = GetDC(hWnd);
      RaytraceScreen(hdc, width, height);
      ReleaseDC(hWnd, hdc);
    }
    break;

    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    BeginPaint(hWnd, &ps);
    // TODO: Add any drawing code here...
    EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}

HitInfo FindClosestHit(const Ray& ray, REAL_TYPE minT, REAL_TYPE maxT)
{
  Vector<HitInfo> hitInfos;

  HitInfo closestHitInfo;
  float closestDistance = maxT;

  // Determining the closest intersection with any sphere in the scene
  for (const auto& sphere : g_sceneSpheres)
  {
    HitInfo hitCandidate;
    if (IntersectRaySphere(ray, sphere, minT, maxT, hitCandidate))
    {
      if (hitCandidate.distance < closestDistance)
      {
        closestDistance = hitCandidate.distance;
        closestHitInfo = hitCandidate;
        closestHitInfo.pSphere = const_cast<Sphere*>(&sphere);
        closestHitInfo.pPlane = nullptr;
        closestHitInfo.pTriangle = nullptr;
      }
    }
  }

  // Determining the closest intersection with any plane in the scene
  for (const auto& plane : g_scenePlanes)
  {
    HitInfo hitCandidate;
    if (IntersectRayPlane(ray, plane, minT, maxT, hitCandidate))
    {
      if (hitCandidate.distance < closestDistance)
      {
        closestDistance = hitCandidate.distance;
        closestHitInfo = hitCandidate;
        closestHitInfo.pPlane = const_cast<Plane*>(&plane);
        closestHitInfo.pSphere = nullptr;
        closestHitInfo.pTriangle = nullptr;
      }
    }
  }

  // Query triangles from scene's octree
  Vector<Triangle*> triangles;
  if (g_sceneOctree != nullptr)
    g_sceneOctree->query(ray, triangles);

  // Determining the closest intersection with any triangle in the scene
  for (const auto& triangle : triangles)
  {
    HitInfo hitCandidate;
    if (IntersectRayTriangle(ray, *triangle, minT, maxT, hitCandidate))
    {
      if (hitCandidate.distance < closestDistance)
      {
        closestDistance = hitCandidate.distance;
        closestHitInfo = hitCandidate;
        closestHitInfo.pPlane = nullptr;
        closestHitInfo.pSphere = nullptr;
        closestHitInfo.pTriangle = const_cast<Triangle*>(triangle);
      }
    }
  }

  return closestHitInfo;
}

Vector4f FindColorForRay(
  const Ray& ray,
  REAL_TYPE minT,
  REAL_TYPE maxT,
  Int32 maxBounces
)
{
  const Vector3f lightPosition(5.0f, 5.0f, -5.0f);
  const Color lightColor(1.0f, 1.0f, 1.0f, 1.0f);
  const Color backgroundColor(0.5f, 0.7f, 1.0f, 1.0f); // Light blue background

  // Radiance accumulation
  Color radiance(0.0f, 0.0f, 0.0f, 1.0f); // Initialize radiance to black

  // Ammount of energy that keeps traveling after each bounce
  Color throughput(1.0f, 1.0f, 1.0f, 1.0f); // Initialize throughput to white

  int depth = 0;
  Ray currentRay = ray;

  while (depth < maxBounces)
  {
    HitInfo closestHitInfo = FindClosestHit(currentRay, minT, maxT);
    if (closestHitInfo.pSphere == nullptr && closestHitInfo.pPlane == nullptr && closestHitInfo.pTriangle == nullptr)
    {
      radiance = radiance + (throughput * backgroundColor); // Accumulate radiance
      break;
    }

    Color objectColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (closestHitInfo.pSphere)
    {
      const Sphere& sphere = *closestHitInfo.pSphere;
      objectColor = sphere.material.albedo;
    }
    else if (closestHitInfo.pPlane)
    {
      const Plane& plane = *closestHitInfo.pPlane;
      objectColor = plane.material.albedo;
    }
    else if (closestHitInfo.pTriangle)
    {
      const Triangle& triangle = *closestHitInfo.pTriangle;
      objectColor = triangle.material.albedo;
    }

    Material& material = closestHitInfo.pSphere ? closestHitInfo.pSphere->material :
                        (closestHitInfo.pPlane ? closestHitInfo.pPlane->material :
                        closestHitInfo.pTriangle->material);

    float reflectivity = std::pow(material.ior - 1.0f, 2.0f) / std::pow(material.ior + 1.0f, 2.0f); // Fresnel reflectance at normal incidence

    const Vector3f lightDir = (lightPosition - closestHitInfo.position).normalized();
    const Vector3f viewDir = currentRay.direction.normalized() * -1.0f;
    const Vector3f halfVector = (lightDir + viewDir).normalized();

    const float NdL = std::max(closestHitInfo.normal.dot(lightDir), 0.0f); // Lambertian reflection
    const float NdH = std::max(closestHitInfo.normal.dot(halfVector), 0.0f); // Blinn-Phong reflection

    float visibility = 0.0f;
    if (NdL > 0.0f)
    {
      visibility = IsInShadow(closestHitInfo, lightPosition) ? 0.0f : 1.0f;
    }

    Color directLight = Color::Black();
    if (visibility)
    {
      directLight = EvaluatePBR(
        closestHitInfo.normal,
        viewDir,
        lightDir,
        closestHitInfo.pSphere ? closestHitInfo.pSphere->material :
                                 (closestHitInfo.pPlane ? closestHitInfo.pPlane->material :
                                 closestHitInfo.pTriangle->material)
      );
      directLight = directLight * lightColor * NdL;
    }

    const Color pbrColor = objectColor * directLight;

    const REAL_TYPE localWeight = 1.0f - reflectivity; // Weight for local color contribution

    if (visibility)
      radiance += throughput * pbrColor * localWeight * visibility; // Accumulate radiance
    else
      radiance += directLight * 0.03f;

    throughput *= reflectivity; // Update throughput for the next bounce

    const REAL_TYPE remainingEnergy = std::max(throughput.r, std::max(throughput.g, throughput.b));
    if (remainingEnergy < 0.001f)
    {
      break; // Terminate the loop if the remaining energy is very low
    }

    Vector3f reflectedDir = currentRay.direction - 2.0f * closestHitInfo.normal * currentRay.direction.dot(closestHitInfo.normal);
    constexpr REAL_TYPE epsilon = 1e-4f; // Small offset to avoid self-intersection
    currentRay = Ray(closestHitInfo.position + reflectedDir * epsilon, reflectedDir); // Offset to avoid self-intersection
    depth++;
  }

  return radiance.vec4;
}

bool IntersectRaySphere(
  const Ray& ray,
  const Sphere& sphere,
  REAL_TYPE minT,
  REAL_TYPE maxT,
  HitInfo& hitInfo
)
{
  Vector3f originToCenter = ray.origin - sphere.center;

  float a = ray.direction.dot(ray.direction); // squared length of the direction vector
  float b = 2.0f * originToCenter.dot(ray.direction); // 2 times the dot product of the vector from the ray origin to the sphere center and the ray direction
  float c = originToCenter.dot(originToCenter) - (sphere.radius * sphere.radius); // squared length of the vector from the ray origin to the sphere center minus the squared radius of the sphere

  // Calculate the discriminant of the quadratic equation to determine if there are real
  // roots (i.e., if the ray intersects the sphere)
  float discriminant = b * b - 4 * a * c; // discriminant of the quadratic equation

  // If the discriminant is negative, there are no real roots, meaning the ray does not
  // intersect the sphere
  if (discriminant < 0)
    return false;

  // Calculate the two possible intersection points (t1 and t2) using the quadratic
  // formula
  float sqrtDiscriminant = sqrtf(discriminant);
  float inverseDenominator = 1.0f / (2.0f * a);
  float distance = (-b - sqrtDiscriminant) * inverseDenominator;

  if (distance < minT || distance > maxT)
  {
    distance = (-b + sqrtDiscriminant) * inverseDenominator;
    if (distance < minT || distance > maxT)
      return false; // Out of bounds, no valid intersection within the specified range
  }

  Vector3f position = ray.where(distance);
  Vector3f normal = (position - sphere.center).normalized();

  hitInfo.distance = distance;
  hitInfo.position = position;
  hitInfo.normal = normal;

  return true;
}

bool IntersectRayPlane(
  const Ray& ray,
  const Plane& plane,
  REAL_TYPE minT,
  REAL_TYPE maxT,
  HitInfo& hitInfo
)
{
  const REAL_TYPE denomitator = plane.normal.dot(ray.direction);

  // check if the ray is parallel to the plane
  if (fabs(denomitator) < 1e-4f)
    return false;

  const REAL_TYPE distance = (plane.point - ray.origin).dot(plane.normal) / denomitator;
  if (distance < minT || distance > maxT)
    return false;

  hitInfo.distance = distance;
  hitInfo.position = ray.where(distance);
  hitInfo.normal = plane.normal;

  return true;
}

/**
 * @brief Determines if a ray intersects with a triangle in 3D space.
 *
 * Using the Möller–Trumbore intersection algorithm.
 *
 * Source:
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle//moller-trumbore-ray-triangle-intersection.html
 */
bool IntersectRayTriangle(
  const Ray& ray,
  const Triangle& triangle,
  REAL_TYPE minT,
  REAL_TYPE maxT,
  HitInfo& hitInfo
)
{
  Vector3f v0v1 = triangle.v1 - triangle.v0;
  Vector3f v0v2 = triangle.v2 - triangle.v0;
  Vector3f pvec = ray.direction.cross(v0v2);

  float det = v0v1.dot(pvec);

  if (det < 1e-8f) return false; // back-face culling

  // for non-culling, use the following line instead:
  //if (std::fabs(det) < 1e-8f) return false;

  float invDet = 1.0f / det;

  Vector3f tvec = ray.origin - triangle.v0;
  float u = tvec.dot(pvec) * invDet;
  if (u < 0.0f || u > 1.0f) return false;

  Vector3f qvec = tvec.cross(v0v1);
  float v = ray.direction.dot(qvec) * invDet;
  if (v < 0.0f || u + v > 1.0f) return false;

  float t = v0v2.dot(qvec) * invDet; // t is the distance from the ray origin to the intersection point
  if (t < minT || t > maxT) return false;

  hitInfo.distance = t;
  hitInfo.position = ray.where(t);
  hitInfo.normal = v0v1.cross(v0v2).normalized();
  //hitInfo.normal = hitInfo.normal * -1.0f; // invert normal
  return true;
}

bool IsInShadow(const HitInfo& hit, const Vector3f& lightPosition)
{
  const REAL_TYPE shadowBias = REAL_TYPE(0.001);
  Ray shadowRay;
  shadowRay.origin = hit.position + hit.normal * shadowBias;

  Vector3f toLight = lightPosition - shadowRay.origin;

  const REAL_TYPE lightDistance = toLight.length();
  if (lightDistance <= shadowBias * REAL_TYPE(2))
    return false;

  shadowRay.direction = toLight * (REAL_TYPE(1) / lightDistance);
  const HitInfo shadowHit = FindClosestHit(shadowRay, shadowBias, lightDistance);
  return (shadowHit.pSphere != nullptr || shadowHit.pPlane != nullptr || shadowHit.pTriangle != nullptr);
}

void RaytraceScreen(HDC hdc, int width, int height)
{
  Vector3f eyePosition(0.0f, 2.0f, -10.0f);
  hc::Matrix4 rotationMatrix = hc::Matrix4::RotationY(hc::Math::DegToRad * g_cameraRotation);
  eyePosition = (rotationMatrix * Vector4f(eyePosition, 1.0f)).xyz();

  const Vector3f camTarget = g_sceneOctree->getBounds().getCenter();
  const Vector3f camUp = Vector3f(0.0f, 1.0f, 0.0f);

  const Vector3f forward = (camTarget - eyePosition).normalized();
  const Vector3f right = camUp.cross(forward).normalized();
  const Vector3f up = forward.cross(right);

  // Projection paramaters
  constexpr float verticalFOV = XMConvertToRadians(60.0f);
  const float nearPlane = 0.1f;
  const float farPlane = 100.0f;
  const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

  const float halfHeight = tanf(verticalFOV / 2.0f);
  const float halfWidth = aspectRatio * halfHeight;

  const float minRayDistance = nearPlane;
  const float maxRayDistance = farPlane;

  // VERSION: antialiasing version + parallelization

  float samplesPerAxis = static_cast<float>(g_antialiasingSamples);
  float samplesPerPixel = samplesPerAxis * samplesPerAxis;

  Vector<Vector4f> pixelColors(width * height, Vector4f(0.0f, 0.0f, 0.0f, 1.0f));

  // parallelization for the outer loop (y-axis), which holds the most independent
  // iterations, to avoid race conditions when writing to pixelColors. Each thread will
  // work on different rows of pixels, so there won't be any conflicts when writing to
  // the pixelColors vector.
#pragma omp parallel for 

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      Vector4f finalColor(0.0f, 0.0f, 0.0f, 1.0f);

      for (int sy = 0; sy < samplesPerAxis; ++sy)
      {
        for (int sx = 0; sx < samplesPerAxis; ++sx)
        {
          const float offsetX = (sy + 0.5f) / samplesPerAxis;
          const float offsetY = (sx + 0.5f) / samplesPerAxis;

          const float pixelX = static_cast<float>(x) + offsetX;
          const float pixelY = static_cast<float>(y) + offsetY;

          // PIXELES NDC COORDINATS

          const float ndcX = (pixelX / static_cast<float>(width)) * 2.0f - 1.0f;
          const float ndcY = 1.0f - (pixelY / static_cast<float>(height) * 2.0f); // Invert Y axis for screen space, direct x coordinates

          // Posición sobre el plano de la imagen local
          const float cameraX = ndcX * halfWidth;
          const float cameraY = ndcY * halfHeight;

          // Dirección del rayo en el espacio del mundo
          Vector3f rayDirection = ((right * cameraX) + (up * cameraY) + forward).normalized();

          Ray ray(eyePosition, rayDirection);
          finalColor = finalColor + FindColorForRay(ray, minRayDistance, maxRayDistance, 4);
        }
      }

      finalColor = finalColor / samplesPerPixel;

      const UInt8 R = std::min(finalColor.x * 255.0f, 255.0f);
      const UInt8 G = std::min(finalColor.y * 255.0f, 255.0f);
      const UInt8 B = std::min(finalColor.z * 255.0f, 255.0f);
      //SetPixel(hdc, x, y, RGB(R, G, B));
      pixelColors[y * width + x] = finalColor;
    }
  }

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      const Vector4f& finalColor = pixelColors[y * width + x];
      const UInt8 R = std::min(finalColor.x * 255.0f, 255.0f);
      const UInt8 G = std::min(finalColor.y * 255.0f, 255.0f);
      const UInt8 B = std::min(finalColor.z * 255.0f, 255.0f);
      SetPixel(hdc, x, y, RGB(R, G, B));
    }
  }

  // VERSION: Normal version, no antialiasing and no parallelization

  /*
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      const float pixelX = static_cast<float>(x) + 0.5f;
      const float pixelY = static_cast<float>(y) + 0.5f;

      // PIXELES NDC COORDINATS

      const float ndcX = (pixelX / static_cast<float>(width)) * 2.0f - 1.0f;
      const float ndcY = 1.0f - (pixelY / static_cast<float>(height) * 2.0f); // Invert Y axis for screen space, direct x coordinates

      // Posición sobre el plano de la imagen local
      const float cameraX = ndcX * halfWidth;
      const float cameraY = ndcY * halfHeight;

      // Dirección del rayo en el espacio del mundo
      Vector3 rayDirection = ((right * cameraX) + (up * cameraY) + forward).normalized();

      Ray ray(eyePosition, rayDirection);
      Vector4 finalColor = FindColorForRay(ray, minRayDistance, maxRayDistance, 3);

      const UInt8 R = min(finalColor.x * 255.0f, 255.0f);
      const UInt8 G = min(finalColor.y * 255.0f, 255.0f);
      const UInt8 B = min(finalColor.z * 255.0f, 255.0f);
      SetPixel(hdc, x, y, RGB(R, G, B));
    }
  }*/
}

void GetTrianglesFromModel(const hc::Model& model, Vector<Triangle>& outTriangles)
{
  const hc::Buffer<hc::Vertex>& vertexBuffer = model.getVertices();
  const hc::Buffer<hc::UInt32>& indexBuffer = model.getIndices();

  for (const hc::ModelSubMesh& mesh : model.getSubMeshes())
  {
    hc::UInt32 numFaces = mesh.indexCount / 3;
    for (hc::UInt32 i = 0; i < numFaces; ++i)
    {
      UInt32 index0 = indexBuffer[mesh.firstIndexIndex + i * 3];
      UInt32 index1 = indexBuffer[mesh.firstIndexIndex + i * 3 + 1];
      UInt32 index2 = indexBuffer[mesh.firstIndexIndex + i * 3 + 2];

      Vector3f v0 = vertexBuffer[index0].position;
      Vector3f v1 = vertexBuffer[index1].position;
      Vector3f v2 = vertexBuffer[index2].position;

      AABB aabb(
        Vector3f(
          std::min(v0.x, std::min(v1.x, v2.x)),
          std::min(v0.y, std::min(v1.y, v2.y)),
          std::min(v0.z, std::min(v1.z, v2.z))
        ),
        Vector3f(
          std::max(v0.x, std::max(v1.x, v2.x)),
          std::max(v0.y, std::max(v1.y, v2.y)),
          std::max(v0.z, std::max(v1.z, v2.z))
        )
      );

      Color triangleColor = Color::Black();
      triangleColor += vertexBuffer[index0].color;
      triangleColor += vertexBuffer[index1].color;
      triangleColor += vertexBuffer[index2].color;
      triangleColor /= 3.0f;

      Triangle triangle(
        v0,
        v1,
        v2,
        Material(triangleColor, 0.3f, 0.8f),
        aabb
      );

      outTriangles.push_back(triangle);
    }
  }
}

AABB ComputeSceneAABB(const Vector<Triangle>& sceneTriangles)
{
  if (sceneTriangles.empty())
    return AABB();

  AABB sceneAABB = sceneTriangles[0].aabb;
  for (const Triangle& triangle : sceneTriangles)
    sceneAABB = AABB::Union(sceneAABB, triangle.aabb);

  return sceneAABB;
}
