
//Set the global scene parameter variables
//TODO: Set the scene parameters based on the values in the scene file

#ifndef PARSE_PGA_H
#define PARSE_PGA_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>

//Camera & Scene Parmaters (Global Variables)
//Here we set default values, override them in parseSceneFile()

//Image Parmaters
int img_width = 800, img_height = 600;
std::string imgName = "raytraced.png";

//Camera Parmaters
Point3D eye = Point3D(0,0,0); 
Dir3D forward = Dir3D(0,0,-1).normalized();
Dir3D up = Dir3D(0,1,0).normalized();
Dir3D right = Dir3D(-1,0,0).normalized();
float halfAngleVFOV = 35; 

//Scene (Sphere) Parmaters
Point3D spherePos = Point3D(0,0,2);
float sphereRadius = 1; 

void parseSceneFile(std::string fileName){
  //TODO: Override the default values with new data from the file "fileName"

  std::ifstream file(fileName);

  if (file.fail()) {
    std::cout << "Can't open file '" << fileName << "'" << std::endl;
  }

  std::string line;
  std::string cmd;
  while (file >> cmd) {     // read first word in line -- the cmd

    if (cmd[0] == '#') {
      std::getline(file, line);   // skip the rest of the line
      std::cout << "Skipping comment: " << cmd  << line <<  std::endl;
      continue;
    }

    if (cmd == "sphere:") {
      float x,y,z,r;
      file >> x >> y >> z >> r;
      spherePos = Point3D(x,y,z);
      sphereRadius = r;
    } 
    else if (cmd == "image_resolution:") {
      int w,h;
      file >> w >> h;
      img_width = w;
      img_height = h;
    } 
    else if (cmd == "output_file:") {
      std::string outFile;
      file >> outFile;
      imgName = outFile;
    } 
    else if (cmd == "camera_pos:") {
      float x,y,z;
      file >> x >> y >> z;
      eye = Point3D(x,y,z);
    } 
    else if (cmd == "camera_fwd:") {
      float fx,fy,fz;
      file >> fx >> fy >> fz;
      forward = Dir3D(fx,fy,fz).normalized();
    } 
    else if (cmd == "camera_up:") {
      float ux,uy,uz;
      file >> ux >> uy >> uz;
      up = Dir3D(ux,uy,uz).normalized();
    } 
    else if (cmd == "camera_fov_ha:") {
      float angle;
      file >> angle;
      halfAngleVFOV = angle;
    } 
    else {
      std::getline(file,line);
      std::cout << "WARNING. Unknow command: " << cmd  << line <<  std::endl;
    }

  }


  right = cross(up,forward);

  right.normalized();
  up.normalized();
  forward.normalized();

  // the vectors should already be normalized

  if ((dot(right,forward) != 0) || (dot(forward,up) != 0) || (dot(up,right) != 0)) {

    // up
    Dir3D new_up = (dot(up,forward) / dot(forward,forward))*forward;
    up = up - new_up;
    up = up.normalized();

    // right
    Dir3D new_right = (dot(right,forward) / dot(forward,forward))*forward;
    right = right - new_right;
    right = right.normalized();

  }

  //TODO: Create an orthagonal camera basis, based on the provided up and right vectors
  printf("Orthagonal Camera Basis:\n");
  forward.print("forward");
  right.print("right");
  up.print("up");

  file.close();
  return;
}

#endif