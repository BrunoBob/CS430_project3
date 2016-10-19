//Theta =0 > not spotlight


Object** object;

for (int y=0; y < M; y += 1) {
  for (int x=0; x < N; x += 1) {
    Ro = ...;
    Rd = ...;

    double closest_t;
    Object* closest_object = NULL;
    double t;
    for (int i=0; object[i] != NULL; i+=1) {

      // object->intersect()
      switch(...) {
        case SPHERE:
        t = sphere_intersect(..);
        break;
        case PLANE:
        t = plane_intersect(..);
        break;
        default:
        // ERROR
        break;
      }

    }
    // We have the closest object..

    double* color = malloc(sizeof(double)*3);
    color[0] = 0; // ambient_color[0];
    color[1] = 0; // ambient_color[1];
    color[2] = 0; // ambient_color[2];

    for (int j=0; light[j] != NULL; j+=1) {
      // Shadow test
      Ron = closest_t * Rd + Ro;
      Rdn = light_position - Ron;
      closest_shadow_object = ...;
      for (int k=0; object[k] != NULL; k+=1) {
        if (object[k] == closest_object) continue;
        //
        switch(...) {
          case SPHERE:
          t = sphere_intersect(..);
          break;
          case PLANE:
          t = plane_intersect(..);
          break;
          default:
          // ERROR
          break;
        }
        if (best_t > distance_to_light) {
          continue;
        }
      }
      if (closest_shadow_object == NULL) {
        // N, L, R, V
        N = closest_object->normal; // plane
        N = Ron - closest_object->center; // sphere
        L = Rdn; // light_position - Ron;
        R = reflection of L;
        V = Rd;
        diffuse = ...; // uses object's diffuse color
        specular = ...; // uses object's specular color
        color[0] += frad() * fang() * (diffuse + specular);
        color[1] += frad() * fang() * (diffuse + specular);
        color[2] += frad() * fang() * (diffuse + specular);
      }
    }
    // The color has now been calculated

    buffer[...].r = (unsigned char)(255 * clamp(color[0]));
    buffer[...].g = (unsigned char)(255 * clamp(color[1]);
    buffer[...].b = (unsigned char)(255 * clamp(color[2]);


  }
}
