

#define SHADER_VARIANT_NV12   0
#define SHADER_VARIANT_RGB    1
#define SHADER_VARIANT_BGR    2
#define SHADER_VARIANT_RGBA   3
#define SHADER_VARIANT_BGRA   4
#define SHADER_VARIANT_NV21   5
#define SHADER_VARIANT_NV16   6
#define SHADER_VARIANT_I420   7
#define SHADER_VARIANT_EXTERNAL 8
#if DEF_VARIANT == SHADER_VARIANT_EXTERNAL
#extension GL_OES_EGL_image_external : require
#endif

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 texCoords;
#if DEF_VARIANT == SHADER_VARIANT_EXTERNAL
uniform samplerExternalOES tex;
#else
uniform sampler2D tex;
uniform sampler2D tex1;
uniform sampler2D tex2;
#endif
uniform int s_TextureFmt;
int c_varant = SHADER_VARIANT_RGB;

vec4 sample_input_texture() {
    vec4 yuva = vec4(0.0, 0.0, 0.0, 1.0);
    if(c_varant==SHADER_VARIANT_RGB || c_varant==SHADER_VARIANT_RGBA) {
        return texture2D(tex,texCoords);
    }
    else if(c_varant==SHADER_VARIANT_BGR || c_varant==SHADER_VARIANT_BGRA) {
        vec4 bgra = texture2D(tex,texCoords);
        return vec4(bgra.b,bgra.g,bgra.r,1.0);
    }
    else if(c_varant==SHADER_VARIANT_NV12) {
        vec3 yuv;
        yuv.r = texture2D(tex,texCoords).r;
#if DEF_VARIANT != SHADER_VARIANT_EXTERNAL
        yuv.g = texture2D(tex1,texCoords).r;
        yuv.b = texture2D(tex1,texCoords).a;
#endif
        mat3 converter = mat3(
                             1.164, 1.164, 1.164,
                             0.000,-0.391, 2.018,
                             1.596,-0.813, 0.000);
        vec3 offset = vec3(0.0, -0.5, -0.5);
        vec3 rgb = converter * (yuv + offset);
        return vec4(rgb,1.0);
    }
    else if(c_varant == SHADER_VARIANT_EXTERNAL) {
#if DEF_VARIANT == SHADER_VARIANT_EXTERNAL
        return texture2D(tex,texCoords);
#endif
    }

}

void main(){
    c_varant = s_TextureFmt;
    vec4 rgb;
    rgb = sample_input_texture();

    gl_FragColor = rgb;
}
