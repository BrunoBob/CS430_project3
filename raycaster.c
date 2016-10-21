#include "json_parser.h"
#include "raycaster.h"

//Print all object detected in json file
void printObjects(objectList list){
  while(list != NULL){
    if(list->kind == 0){
      printf("Object of kind : sphere\n");
      printf("Diffuse color : %lf  %lf  %lf\n", list->diffuseColor[0], list->diffuseColor[1], list->diffuseColor[2]);
      printf("Specular color : %lf  %lf  %lf\n", list->specularColor[0], list->specularColor[1], list->specularColor[2]);
      printf("Position : %lf  %lf  %lf\n", list->position[0], list->position[1], list->position[2]);
      printf("Radius : %lf\n", list->sphere.radius);
    }
    else{
      printf("Object of kind : plane\n");
      printf("Diffuse color : %lf  %lf  %lf\n", list->diffuseColor[0], list->diffuseColor[1], list->diffuseColor[2]);
      printf("Specular color : %lf  %lf  %lf\n", list->specularColor[0], list->specularColor[1], list->specularColor[2]);
      printf("Position : %lf  %lf  %lf\n", list->position[0], list->position[1], list->position[2]);
      printf("Normal : %lf  %lf  %lf\n", list->plane.normal[0], list->plane.normal[1], list->plane.normal[2]);
    }
    printf("\n");
    list = list->next;
  }
}

//Print all lights detected in json file
void printLights(lightList list){
  while (list != NULL) {
    if(list->theta == 0){
      printf("\n\n Point light\n");
    }
    else{
      printf("\n\n spotlight\n");
    }
    printf("Light : \nLights color : %lf  %lf  %lf\n", list->color[0], list->color[1], list->color[2]);
    printf("Position : %lf  %lf  %lf\n", list->position[0], list->position[1], list->position[2]);
    printf("Direction : %lf  %lf  %lf\n", list->direction[0], list->direction[1], list->direction[2]);
    printf("Radial-a0 : %lf\n", list->radA0);
    printf("Radial-a0 : %lf\n", list->radA1);
    printf("Radial-a0 : %lf\n", list->radA2);
    if(list->theta != 0){
      printf("Theta : %lf\n", list->theta);
      printf("Angular-a0 : %lf\n", list->angA0);
    }

    printf("\n");
    list = list->next;
  }
}


//Compute if interserction with a plane
double planeIntersection(double* Ro, double* Rd, double* position, double* normal){
  double t = 0;
  normalize(normal); //Just in case
  double denom = dotProduct(normal, Rd);
  if(sqrt(sqr(denom)) > 0.00001){
    t = (-dotProduct(subVector(Ro, position), normal)) / denom;
  }
  return t;
}

//Compute if interserction with a sphere
double sphereIntersection(double* Ro, double* Rd, double* position, double radius){
  double t;

  double* RoSubPosition = subVector(Ro, position);
  double b = 2 * dotProduct(Rd, RoSubPosition);
  double c = dotProduct(RoSubPosition, RoSubPosition) - radius;

  double sqrtDelta = sqrt(sqr(b) - 4 * c);

  t = (-b - sqrtDelta) / 2;

  if(t < 0){
    t = (-b + sqrtDelta) / 2;
  }

  return t;
}


//Write the data in a P6 ppm file
void createScene(char* ppm, unsigned char* data, int width, int height){
  FILE* outputFile = fopen(ppm, "w");

  if (outputFile == NULL) {
    fprintf(stderr, "Error: Could not open file \"%s\"\n", ppm);
    exit(ERROR_WRITING);
  }

  if(fprintf(outputFile, "P6\n#Written by raycaster program made by Bruno TESSIER\n%d %d\n255\n", width, height) < 63){
    fprintf(stderr, "Error: Could not write header in file \"%s\"\n", ppm);
    exit(ERROR_WRITING);
  }
  if(fwrite(data, sizeof(char), width * height * 3, outputFile) != (width * height * 3)){
    fprintf(stderr, "Error: Could not write data in file \"%s\"\n", ppm);
    exit(ERROR_WRITING);
  }
  fclose(outputFile);
}
double fAng(double* V0, double* Vl, double angleMax, double a0){
  double dot = dotProduct(V0, Vl);
  if(acos(dot) > angleMax){
    return 0;
  }
  return pow(dot, a0);
}

double fRad(double dist, double a0, double a1, double a2){
  if(dist == INFINITY){
    return 1;
  }
  return 1/(a2*sqr(dist + a1*dist + a0));
}


int main(int argc, char *argv[]){
  if(argc < 5){
    fprintf(stderr, "Error: Expected ./raycaster width height input.json output.ppm");
    exit(ERROR_RAYCAST);
  }

  double centerX = 0;
  double centerY = 0;

  int width = atoi(argv[1]);
  int height = atoi(argv[2]);

  double camWidth, camHeight;

  components comp = NULL;
  comp = parseFile(argv[3], &camWidth, &camHeight);
  objectList list = comp->objects;
  lightList lights = comp->lights;

  double pixWidth = camWidth / width;
  double pixHeight = camHeight / height;

  printf("\nScene : width = %d\theight = %d\n", width, height);
  printf("\nCamera : width = %lf\theight = %lf\n\n", camWidth, camHeight);
  printObjects(list);
  printLights(lights);
  unsigned char* data = (unsigned char*)malloc(width * height * 3 * sizeof(unsigned char));

  int x,y;

  for(y = 0; y < height ; y++){
    for(x = 0; x < width ; x++){
      double* Ro = getVector(0, 0, 0);
      double Rx = centerX - (camWidth/2) + pixWidth * (x+0.5);
      double Ry = centerY - (camHeight/2) + pixHeight * (y+0.5);
      double* Rd = getVector(Rx, Ry, 1);

      normalize(Rd);

      double bestT = INFINITY;
      objectList tempList = list;
      double *pixel;
      while (tempList != NULL) {
        double t=0;

        switch (tempList->kind) {
          case 0:
          t = sphereIntersection(Ro, Rd, tempList->position, tempList->sphere.radius);
          break;
          case 1:
          t = planeIntersection(Ro, Rd, tempList->position, tempList->plane.normal);
          break;
          default:
          fprintf(stderr, "Error: Object of kind unknow (How is it even possible ?)");
          exit(ERROR_RAYCAST);
        }

        if(t > 0 && t < bestT){
          bestT = t;
          pixel = tempList->diffuseColor;
        }
        tempList = tempList->next;
      }
      if(bestT > 0 && bestT != INFINITY){
        data[ 3 * (x + width * (height - 1 - y))] = pixel[0] * 255;
        data[ 3 * (x + width * (height - 1 - y)) + 1] = pixel[1] * 255;
        data[ 3 * (x + width * (height - 1 - y)) + 2] = pixel[2] * 255;
      }
      else{
        data[ 3 * (x + width * (height - 1 - y))] = 0;
        data[ 3 * (x + width * (height - 1 - y)) + 1] = 0;
        data[ 3 * (x + width * (height - 1 - y)) + 2] = 0;
      }
    }
  }

  createScene(argv[4], data, width, height);
  free(data);

  return 0;
}
