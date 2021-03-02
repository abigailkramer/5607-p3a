
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
int img_width = 640, img_height = 480;
std::string imgName = "raytraced.bmp";

//Camera Parmaters
Point3D eye = Point3D(0,0,0); 
Dir3D forward = Dir3D(0,0,1).normalized();
Dir3D up = Dir3D(0,1,0).normalized();
Dir3D right = Dir3D(-1,0,0).normalized();
float halfAngleVFOV = 45; 

//Scene Parameters
int max_vertices = 0;
int max_normals = 0;
Point3D *vertex;                // need to delete somewhere
int vert_pos = 0;
MultiVector *normal;
int norm_pos = 0;

// add triangle (v1,v2,v2) & normal_triangle (v1,v2,v3,n1,n2,n3)

Point3D spherePos = Point3D(0,0,2); // maybe change?? we'll need multiple
float sphereRadius = 1;
Color background = Color(0,0,0);

//Material Parameters
Color ambient_color = Color(0,0,0);
Color diffuse_color = Color(1,1,1);
Color specular_color = Color(0,0,0);
int ns = 0;
Color transmissive_color = Color(0,0,0);
float ior;

//Lighting Parameters
Color dir_intensity;  // is Color the right type?
Dir3D dir_direction;

Color point_intensity;
Dir3D point_direction;

Color spot_intensity;
Point3D spot_location;
Dir3D spot_direction;
float angle1;         // points at angles < angle1, light behaves like point
float angle2;         // points at angles > angle2, light contributes nothing
                      // points in between, light should fall off smoothly (linear is fine)

Color ambient_light = Color(0,0,0);

//Misc.
int max_depth = 5;


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


    if (cmd == "camera_pos:") {
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
    else if (cmd == "film_resolution:") {
      int width,height;
      file >> width >> height;
      img_width = width;
      img_height = height;
    } 
    else if (cmd == "output_image:") {
      std::string outFile;
      file >> outFile;
      imgName = outFile;
    }
// Scene Geometry
    else if (cmd == "max_vertices:") {
      int max;
      file >> max;
      max_vertices = max;
      vertex = new Point3D[max_vertices];
    }
    else if (cmd == "max_normals:") {
      int max;
      file >> max;
      max_normals = max;
      normal = new MultiVector[max_normals];
    }
    else if (cmd == "vertex:") {
      float x,y,z;
      file >> x >> y >> z;
      vertex[vert_pos] = Point3D(x,y,z);
      vert_pos++;
    }
    else if (cmd == "normal:") {
      float x,y,z;
      file >> x >> y >> z;
      normal[norm_pos] = MultiVector(x,y,z);
      norm_pos++;
    }
    else if (cmd == "triangle:") {
      int v1,v2,v3;
      file >> v1 >> v2 >> v3;
      // add triangle using vertex[v1], vertex[v2], and vertex[v3]
    }
    else if (cmd == "normal_triangle:") {
      int v1,v2,v3,n1,n2,n3;
      file >> v1 >> v2 >> v3 >> n1 >> n2 >> n3;
      // add normal_triangle using vertex[v1], vertex[v2], and vertex[v3]
      // with normals normal[n1], normal[n2], and normal[n3]
    }
    else if (cmd == "sphere:") {
      float x,y,z,r;
      file >> x >> y >> z >> r;
      spherePos = Point3D(x,y,z);
      sphereRadius = r;
    }
    else if (cmd == "background:") {
      float r,g,b;
      file >> r >> g >> b;
      background = Color(r,g,b);
    }
// Material Parameters
    else if (cmd == "material:") {
      float ar,ag,ab,dr,dg,db,sr,sg,sb,tr,tg,tb,io;
      int n;
      file >> ar >> ag >> ab >> dr >> dg >> db;
      file >> sr >> sg >> sb >> n >> tr >> tg >> tb >> io;
      ambient_color = Color(ar,ag,ab);
      diffuse_color = Color(dr,dg,db);
      specular_color = Color(sr,sg,sb);
      transmissive_color = Color(tr,tg,tb);
      ns = n;
      ior = io;
    }
// Lighting Parameters
    else if (cmd == "directional_light:") {
      float r,g,b,x,y,z;
      file >> r >> g >> b >> x >> y >> z;
      dir_intensity = Color(r,g,b);
      dir_direction = Dir3D(x,y,z);
    }
    else if (cmd == "point_light:") {
      float r,g,b,x,y,z;
      file >> r >> g >> b >> x >> y >> z;
      point_intensity = Color(r,g,b);
      point_direction = Dir3D(x,y,z);
    }
    else if (cmd == "spot_light:") {
      float r,g,b,px,py,pz,dx,dy,dz,a1,a2;
      file >> r >> g >> b >> px >> py >> pz;
      file >> dx >> dy >> dz >> a1 >> a2;
      spot_intensity = Color(r,g,b);
      spot_location = Point3D(px,py,pz);
      spot_direction = Dir3D(dx,dy,dz);
      angle1 = a1;
      angle2 = a2;
    }
    else if (cmd == "ambient_light:") {
      float r,g,b;
      file >> r >> g >> b;
      ambient_light = Color(r,g,b);
    }
// Miscellaneous
    else if (cmd == "max_depth:") {
      int n;
      file >> n;
      max_depth = n;
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