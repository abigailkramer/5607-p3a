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

//#3D PGA
#include "PGA_3D.h"

//High resolution timer
#include <chrono>

//Scene file parser
#include "parse_pga.h"

bool raySphereIntersect_fast(Point3D rayStart, Line3D rayLine, Point3D sphereCenter, float sphereRadius){
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
    if (t0 > 0 || t1 > 0) return true;
  }
  return false;
}

bool raySphereIntersect(Point3D rayStart, Line3D rayLine, Point3D sphereCenter, float sphereRadius){
  Point3D projPoint = dot(rayLine,sphereCenter)*rayLine;      //Project to find closest point between circle center and line [proj(sphereCenter,rayLine);]
  float distSqr = projPoint.distToSqr(sphereCenter);          //Point-line distance (squared)
  float d2 = distSqr/(sphereRadius*sphereRadius);             //If distance is larger than radius, then...
  if (d2 > 1) return false;                                   //... the ray missed the sphere
  float w = sphereRadius*sqrt(1-d2);                          //Pythagorean theorem to determine dist between proj point and intersection points
  Point3D p1 = projPoint - rayLine.dir()*w;                   //Add/subtract above distance to find hit points
  Point3D p2 = projPoint + rayLine.dir()*w; 

  if (dot((p1-rayStart),rayLine.dir()) >= 0) return true;     //Is the first point in same direction as the ray line?
  if (dot((p2-rayStart),rayLine.dir()) >= 0) return true;     //Is the second point in same direction as the ray line?
  return false;
}

Color ApplyLightingModel(Point3D rayStart, Line3D rayLine, Point3D hit_point, Dir3D normal, Sphere sphere) {  // Scene scene, Ray ray, HitInformation hit
  Color contribution = Color(0,0,0);

  for (auto& dl : dir_lights) {
    Line3D shadow = vee(hit_point,dl.direction).normalized();

  }

  for (auto& pl : point_lights) {
    Dir3D lightDir = (pl.location - hit_point);
    Line3D shadow = vee(hit_point,lightDir).normalized();
    // HitInformation shadow_hit;
    bool blocked;
    // bool blocked = FindIntersection(scene, shadow, &shadow_hit);
    // rn go through spheres and see if shadow intersects
      // would need to go through all primitives in scene
    for (auto& s : spheres) {
      bool blocked = raySphereIntersect(eye,rayLine,s.pos,s.radius);
      // shadow_hit.t -- quadratic equation solution
    }
    
    if (blocked) { // && (shadow_hit.t < Distance(Light.pos - hit_point))
      continue;    // in shadow - move to next light
    }
    // dot(shadow,normal)*sphere.diffuse;
  }

  for (auto& sl : spot_lights) {
    
  }

  // for each light L
    // create a ray to the light
    // w/ the hit_point as origin
    // the direction is L.pos - hit_point
  // check for intersection w/ light
  // if none, skip specular & diffuse (i think kinda rare?)


  // go through direct, point, spot, and ambient lights

  return background;
}

Color EvaluateRayTree(Point3D rayStart, Line3D rayLine) {
  bool hit_something;
  // HitInformation hit;      // struct containing hit point, normal, etc

  // hit_something = FindIntersection(scene, ray, &hit);
  // ^ need to go through objects in the scene (look at closer objects first)

  for (auto& s : spheres) {
    hit_something = raySphereIntersect(eye,rayLine,s.pos,s.radius);

    if (hit_something) {
      // get hit point
      Point3D projPoint = dot(rayLine,s.pos)*rayLine;
      float distSqr = projPoint.distToSqr(s.pos);
      float d2 = distSqr/(s.radius*s.radius);
      float w = s.radius*sqrt(1-d2);
      Point3D hit = projPoint + rayLine.dir()*w;        // + or - ?

      // get normal
      Dir3D norm = (hit - s.pos).normalized();

      return ApplyLightingModel(rayStart,rayLine,hit,norm,s);
    } else {
      return background;
    }    
  }

  return background;

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

      // intersect w/ scene
      // no intersect? -> set pixel to background color

      // EvaluateRayTree - return the color to set pixel to
      // Color color = EvaluateRayTree(eye,rayLine);

      for (auto& s : spheres) {
        bool hit = raySphereIntersect(eye,rayLine,s.pos,s.radius);
        if (hit) outputImg.setPixel(i,j,s.ambient);
      }

      // for (int sphere = 0; sphere < numSpheres; sphere++) { // this for loop put somewhere in EvaluateRayTree
      //   Sphere s = spheres[sphere];
      //   bool hit = raySphereIntersect(eye,rayLine,s.pos,s.radius);
      //   Color color;
      //   if (hit) outputImg.setPixel(i,j,s.ambient);   // need more nuance w/ light --> very basic here
      // }

      // compute illumination @ visual point
    }
  }

  auto t_end = std::chrono::high_resolution_clock::now();
  printf("Rendering took %.2f ms\n",std::chrono::duration<double, std::milli>(t_end-t_start).count());

  outputImg.write(imgName.c_str());
  return 0;
}
