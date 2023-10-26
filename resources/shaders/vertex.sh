attribute vec3 aPos;
attribute vec2 ayTexCoords;
varying vec2 texCoords;

void main(){
     gl_Position = vec4(aPos,1.0);
     texCoords = ayTexCoords;
}
