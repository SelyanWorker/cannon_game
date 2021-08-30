#type vertex
#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec2 normal;

uniform mat3 textureMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 outTexCoord;

void main()
{
	outTexCoord = (textureMatrix * vec3(texCoord, 1)).xy;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1);
	//gl_Position = vec4(position, 1);
}


#type fragment
#version 400 core

in vec2 outTexCoord;
uniform sampler2D sampler;

out vec4 fragColor;

uniform vec4 color;

void main()
{
	fragColor = texture(sampler, outTexCoord);
	//fragColor = color;
	//fragColor = vec4(outTexCoord.g, 0, 0, 1);
}
