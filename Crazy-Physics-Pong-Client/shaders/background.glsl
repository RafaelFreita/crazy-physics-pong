#version 130

uniform vec2 lightOrigin;
uniform vec3 lightColor;
uniform float lightAttenuation;

uniform vec2 screenResolution;

void main() {
	vec2 baseDistance = gl_FragCoord.xy;
	//baseDistance.y = frag_ScreenResolution.y-baseDistance.y;

	vec2 distance = lightOrigin - baseDistance;
	float linear_distance = length(distance);
	float attenuation= 1.0 / ( lightAttenuation * linear_distance + lightAttenuation * linear_distance);

	vec4 lightColor = vec4(lightColor, 1.0);
	gl_FragColor = vec4(attenuation, attenuation, attenuation, 1.0) * lightColor;

}