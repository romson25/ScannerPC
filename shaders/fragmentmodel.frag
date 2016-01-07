varying vec3 normalInterp;
varying vec3 vertPos;

//const vec3 lightPos     = vec3(1.0, 1.0, 1.0);
const vec3 lightPos     = vec3(5.0, 5.0, 5.0);
//const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 diffuseColor = vec3(1.0, 1.0, 0.94);
//const vec3 specColor    = vec3(1.0, 1.0, 1.0);
const vec3 specColor    = vec3(0.19, 0.189, 0.189);

void main()
{
    vec3 normal = normalize(normalInterp);
    vec3 lightDir = normalize(lightPos - vertPos);

    float lambertian = max(dot(lightDir,normal), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0) {

      vec3 reflectDir = reflect(-lightDir, normal);
      vec3 viewDir = normalize(-vertPos);

      float specAngle = max(dot(reflectDir, viewDir), 0.0);
      specular = pow(specAngle, 4.0);
    }

    gl_FragColor = vec4( lambertian*diffuseColor + specular*specColor, 1.0);
}
