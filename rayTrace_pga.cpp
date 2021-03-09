//CSCI 5607 HW3 - Rays & Files
//This HW has three steps:
// 1. Compile and run the program (the program takes a single command line argument)
// 2. Understand the code in this file (rayTrace_pga.cpp), in particular be sure to understand:
//     -How ray-sphere intersection works
//     -How the rays are being generated
//     -The pipeline from rays, to intersection, to pixel color
//    After you finish this step, and understand the math, take the HW quiz on canvas
// 3. Update the file parse_pga.h so that the function parseSceneFile() reads the passed in file
//     and sets the relevant global variables for the rest of the code to product to correct image

//To Compile: g++ -fsanitize=address -std=c++11 rayTrace_pga.cpp

//For Visual Studios
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

//Images Lib includes:
#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files
#include "image_lib.h" //Defines an image class and a color class

//enables use of M_PI
#define _USE_MATH_DEFINES
#define INF ((unsigned) ~0)

//#3D PGA
#include "PGA_3D.h"

//High resolution timer
#include <chrono>

//Scene file parser
#include "parse_pga.h"

bool raySphereIntersect_fast(Point3D rayStart, Line3D rayLine, Point3D sphereCenter, float sphereRadius, HitInformation *hit){
  Dir3D dir = rayLine.dir();
  float a = dot(dir,dir);
  Dir3D toStart = (rayStart - sphereCenter);
  float b = 2 * dot(dir,toStart);
  float c = dot(toStart,toStart) - sphereRadius*sphereRadius;
  float discr = b*b - 4*a*c;
  if (discr < 0) return false;
  else{

    float t0 = (-b + sqrt(discr))/(2*a);
    float t1 = (-b - sqrt(discr))/(2*a);

    if (t0 > 0) {
      hit->t = t0;
      // hit->hit_point = rayStart + dir*t0;
      // hit->normal = (hit->hit_point - sphereCenter).normalized();
    } else if (t1 > 0) {
      hit->t = t1;
      // hit->hit_point = rayStart + dir*t1;
      // hit->normal = (hit->hit_point - sphereCenter).normalized();
    }

    if (t0 > 0 || t1 > 0) return true;
  }
  return false;
}

bool raySphereIntersect(Point3D rayStart, Line3D rayLine, Point3D sphereCenter, float sphereRadius, HitInformation *hit) {
  Point3D projPoint = dot(rayLine,sphereCenter)*rayLine;      //Project to find closest point between circle center and line [proj(sphereCenter,rayLine);]
  float distSqr = projPoint.distToSqr(sphereCenter);          //Point-line distance (squared)
  float d2 = distSqr/(sphereRadius*sphereRadius);             //If distance is larger than radius, then...
  if (d2 > 1) return false;                                   //... the ray missed the sphere
  float w = sphereRadius*sqrt(1-d2);                          //Pythagorean theorem to determine dist between proj point and intersection points
  Point3D p1 = projPoint - rayLine.dir()*w;                   //Add/subtract above distance to find hit points
  Point3D p2 = projPoint + rayLine.dir()*w; 

  if (dot((p1-rayStart),rayLine.dir()) >= 0){
    hit->hit_point = p1;
    hit->t = w;
    return true;     //Is the first point in same direction as the ray line?
  }
  if (dot((p2-rayStart),rayLine.dir()) >= 0){    
    // hit->t = w;
    hit->hit_point = p2;
    hit->t = (hit->hit_point - rayStart).magnitude();
    return true;     //Is the second point in same direction as the ray line?
  }
  return false;
}

Color ApplyLightingModel(Point3D rayStart, Line3D rayLine,HitInformation hitInfo) {  // Scene scene, Ray ray, HitInformation hit
  // start with ambient_light * ambient response
  float r_cont = (ambient_light.r*hitInfo.ambient.r);
  float g_cont = (ambient_light.g*hitInfo.ambient.g);
  float b_cont = (ambient_light.b*hitInfo.ambient.b);

  for (auto& light : dir_lights) {
    Point3D p = hitInfo.hit_point;
    Dir3D lightDir = light.direction;

    // shadows
    Line3D shadow = vee(hitInfo.hit_point,light.direction).normalized();

    Dir3D N = hitInfo.normal;
    Dir3D L = lightDir.normalized();
    float n_l = std::max(dot(N,L),0.f);
    
    Dir3D V = (eye - p).normalized();
    Dir3D R = (L - 2*(dot(L,N)*N)).normalized();
    float v_r = pow(std::max(dot(V,R),0.f), hitInfo.ns);

    float contribute_r = ((n_l*hitInfo.diffuse.r) + (v_r*hitInfo.specular.r))*light.intensity.r;
    float contribute_g = ((n_l*hitInfo.diffuse.g) + (v_r*hitInfo.specular.g))*light.intensity.g;
    float contribute_b = ((n_l*hitInfo.diffuse.b) + (v_r*hitInfo.specular.b))*light.intensity.b;

    r_cont += contribute_r;
    g_cont += contribute_g;
    b_cont += contribute_b;
  }

  for (auto& light : point_lights) {     // get it so it drops off w/ distance
    Point3D p = hitInfo.hit_point;
    Dir3D lightDir = (light.location - p).normalized();

    Line3D shadow = vee(p,lightDir).normalized();
    HitInformation shadow_hit = HitInformation();
    bool blocked = false;

    for (auto& s : spheres) {
      if (raySphereIntersect(p,shadow,s.pos,s.radius,&shadow_hit)) {
        blocked = true;
      }
    }

    double bias = 0.01;
    double dist = p.distTo(light.location);    
    if (blocked && (shadow_hit.t < dist) && shadow_hit.t > bias) continue;

    Dir3D N = hitInfo.normal;
    Dir3D L = lightDir.normalized();
    float n_l = std::max(dot(N,L),0.f);              // between direction of the light source & surface normal
    
    Dir3D V = (eye - p).normalized();
    Dir3D R = (L - 2*(dot(L,N)*N)).normalized();
    float v_r = pow(std::max(dot(V,R),0.f), hitInfo.ns);

    float contribute_r = ((n_l * hitInfo.diffuse.r)) + ((v_r * hitInfo.specular.r));
    float contribute_g = ((n_l * hitInfo.diffuse.g)) + ((v_r * hitInfo.specular.g));
    float contribute_b = ((n_l * hitInfo.diffuse.b)) + ((v_r * hitInfo.specular.b));

    float attenuation = 1.0 / (1.0 + dist*dist);

    contribute_r *= light.intensity.r*attenuation;
    contribute_g *= light.intensity.g*attenuation;
    contribute_b *= light.intensity.b*attenuation;

    r_cont += contribute_r;
    g_cont += contribute_g;
    b_cont += contribute_b;
  }

  for (auto& light : spot_lights) {
    Point3D p = hitInfo.hit_point;
    Dir3D lightDir = (light.location - p);

    // shadows
    float dist = p.distTo(light.location);
    // check angle range for shadows?? since it illumination = 0 if not in range

    Dir3D N = hitInfo.normal;
    Dir3D L = lightDir.normalized();
    float n_l = std::max(dot(N,L),0.f);              // between direction of the light source & surface normal
    
    Dir3D V = (eye - p).normalized();
    Dir3D R = (L - 2*(dot(L,N)*N)).normalized();
    float v_r = pow(std::max(dot(V,R),0.f), hitInfo.ns);

    float contribute_r = ((n_l * hitInfo.diffuse.r)) + ((v_r * hitInfo.specular.r));
    float contribute_g = ((n_l * hitInfo.diffuse.g)) + ((v_r * hitInfo.specular.g));
    float contribute_b = ((n_l * hitInfo.diffuse.b)) + ((v_r * hitInfo.specular.b));

    float attenuation = 1.0 / (1.0 + dist*dist);

    contribute_r *= light.intensity.r*attenuation;      // only if w/i certain angle range
    contribute_g *= light.intensity.g*attenuation;
    contribute_b *= light.intensity.b*attenuation;

    r_cont += contribute_r;
    g_cont += contribute_g;
    b_cont += contribute_b;
  }

  return Color(r_cont,g_cont,b_cont);
}

Color EvaluateRayTree(Point3D rayStart, Line3D rayLine) {
  double currentDist = INF;                              // start as far away as possible
  HitInformation info = HitInformation();
  HitInformation possible = HitInformation();
  bool hit = false;

  for (auto& s : spheres) {
    if (raySphereIntersect(eye,rayLine,s.pos,s.radius,&possible)) {

      if (eye.distTo(possible.hit_point) > currentDist) {   // move from back to front
        continue;
      }
      currentDist = eye.distTo(possible.hit_point);

      hit = true;
      info.hit_point = possible.hit_point;
      info.t = possible.t;
      info.normal = (info.hit_point - s.pos).normalized();
      info.ambient = s.ambient;
      info.diffuse = s.diffuse;
      info.specular = s.specular;
      info.transmissive = s.transmissive;
      info.ns = s.ns;
      info.ior = s.ior;
    }
  }

  if (hit) {
    Color color = ApplyLightingModel(eye,rayLine,info);
    return color;
  } else {
    return background;
  }
}


int main(int argc, char** argv){
  
  //Read command line paramaters to get scene file
  if (argc != 2){
     std::cout << "Usage: ./a.out scenefile\n";
     return(0);
  }
  std::string secenFileName = argv[1];

  //Parse Scene File
  parseSceneFile(secenFileName);

  float imgW = img_width, imgH = img_height;
  float halfW = imgW/2, halfH = imgH/2;
  float d = halfH / tanf(halfAngleVFOV * (M_PI / 180.0f));

  Image outputImg = Image(img_width,img_height);
  auto t_start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < img_width; i++) {       // just a placeholder rn
    for (int j = 0; j < img_height; j++) {
      outputImg.setPixel(i,j,background);
    }
  }

  for (int i = 0; i < img_width; i++) {
    for (int j = 0; j < img_height; j++) {
      float u = (halfW - (imgW)*((i+0.5)/imgW));
      float v = (halfH - (imgH)*((j+0.5)/imgH));
      Point3D p = eye - d*forward + u*right + v*up;
      Dir3D rayDir = (p - eye); 
      Line3D rayLine = vee(eye,rayDir).normalized();  //Normalizing here is optional

      Color color = EvaluateRayTree(eye,rayLine);
      outputImg.setPixel(i,j,color);

    }
  }

  auto t_end = std::chrono::high_resolution_clock::now();
  printf("Rendering took %.2f ms\n",std::chrono::duration<double, std::milli>(t_end-t_start).count());

  outputImg.write(imgName.c_str());
  return 0;
}
