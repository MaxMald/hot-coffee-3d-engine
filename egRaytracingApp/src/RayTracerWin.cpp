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

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

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

const UInt32 g_antialiasingSamples = 4;
const float g_cameraRotation = 30.0f;
//const String g_modelPath = "spunky/Spunky.obj";
const String g_modelPath = "spaceships-scene/spaceships-scene.obj";
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

  g_sceneSpheres.push_back(
    Sphere(
      Vector3f(3.0f, 0.0f, 0.0f),
      1.0f,
      Color::Red(),
      0.3,
      0.8,
      0.2
    )
  );

  g_sceneSpheres.push_back(
    Sphere(
      Vector3f(3.0f, 5.0f, 0.0f),
      1.0f,
      Color(1.0f, 1.0f, 0.0f, 1.0f),
      0.3,
      0.8,
      0.2
    )
  );

  g_sceneSpheres.push_back(
    Sphere(
      Vector3f(-3.0f, 0.0f, 0.0f),
      1.0f,
      Color::Green(),
      0.3,
      0.8,
      0.2
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
      Color::Blue(),
      0.3,
      0.8,
      0.2
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
  const Vector4f lightColor(1.0f, 1.0f, 1.0f, 1.0f);
  const Vector4f backgroundColor(0.5f, 0.7f, 1.0f, 1.0f); // Light blue background

  // Radiance accumulation
  Vector4f radiance(0.0f, 0.0f, 0.0f, 1.0f); // Initialize radiance to black

  // Ammount of energy that keeps traveling after each bounce
  Vector4f throughput(1.0f, 1.0f, 1.0f, 1.0f); // Initialize throughput to white

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

    Vector4f objectColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (closestHitInfo.pSphere)
    {
      const Sphere& sphere = *closestHitInfo.pSphere;
      objectColor = sphere.color.vec4;
    }
    else if (closestHitInfo.pPlane)
    {
      const Plane& plane = *closestHitInfo.pPlane;
      objectColor = plane.color.vec4;
    }
    else if (closestHitInfo.pTriangle)
    {
      const Triangle& triangle = *closestHitInfo.pTriangle;
      objectColor = triangle.color.vec4;
    }

    constexpr REAL_TYPE ambientFactor = 0.05f; // Ambient light factor
    constexpr REAL_TYPE diffuseFactor = 0.95f; // Diffuse reflection factor
    constexpr REAL_TYPE shininess = 32.0f; // Shininess factor for specular reflection
    REAL_TYPE reflectivity = 0.3f; // Specular coefficient

    if (closestHitInfo.pPlane != nullptr)
      reflectivity = 0.0f;

    const Vector3f lightDir = (lightPosition - closestHitInfo.position).normalized();
    const Vector3f viewDir = currentRay.direction.normalized() * -1.0f;
    const Vector3f halfVector = (lightDir + viewDir).normalized();

    const REAL_TYPE NdL = std::max(closestHitInfo.normal.dot(lightDir), 0.0f); // Lambertian reflection
    const REAL_TYPE NdH = std::max(closestHitInfo.normal.dot(halfVector), 0.0f); // Blinn-Phong reflection

    REAL_TYPE visibility = 0.0f;
    if (NdL > 0.0f)
    {
      visibility = IsInShadow(closestHitInfo, lightPosition) ? 0.0f : 1.0f;
    }

    const REAL_TYPE diffuse = ambientFactor + diffuseFactor * NdL; // Diffuse
    const REAL_TYPE specular = powf(NdH, shininess); // Specular

    const Vector4f diffuseColor = objectColor * diffuse;
    const Vector4f specularColor = lightColor * specular;

    const REAL_TYPE localWeight = 1.0f - reflectivity; // Weight for local color contribution
    radiance += throughput * (diffuseColor + specularColor) * localWeight * visibility; // Accumulate radiance
    throughput *= reflectivity; // Update throughput for the next bounce

    const REAL_TYPE remainingEnergy = std::max(throughput.x, std::max(throughput.y, throughput.z));
    if (remainingEnergy < 0.001f)
    {
      break; // Terminate the loop if the remaining energy is very low
    }

    Vector3f reflectedDir = currentRay.direction - 2.0f * closestHitInfo.normal * currentRay.direction.dot(closestHitInfo.normal);
    constexpr REAL_TYPE epsilon = 1e-4f; // Small offset to avoid self-intersection
    currentRay = Ray(closestHitInfo.position + reflectedDir * epsilon, reflectedDir); // Offset to avoid self-intersection
    depth++;
  }

  return radiance;
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
        triangleColor,
        0.3f,
        0.8f,
        0.2f,
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
