#version 330 core
in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
   float alpha = texture(text, TexCoords).r * textColor.a;
   gl_FragColor = vec4(textColor.rgb, alpha); // Use textColor for RGB and sampled alpha
}
