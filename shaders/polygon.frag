#version 330 core
uniform vec4 uColor; // Uniform color for the entire rectangle
uniform sampler2D uTexture;
uniform int uSampleTexture = 0;

in vec2 TexCoords;

void main()
{
   if (uSampleTexture == 1)
   {
      gl_FragColor = texture(uTexture, TexCoords) * uColor;
   }
   else
   {
      gl_FragColor = uColor;
   }
}
