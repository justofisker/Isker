#version 300 es
precision mediump float;

in vec2  v_UV;
in vec4  v_Color;
in float v_Texure;

out vec4 FragColor;

uniform sampler2D u_Textures[16];

void main()
{
    FragColor = v_Color;

    switch(int(v_Texure))
	{
		case 0: FragColor *= texture(u_Textures[0], v_UV); break;
		case 1: FragColor *= texture(u_Textures[1], v_UV); break;
		case 2: FragColor *= texture(u_Textures[2], v_UV); break;
		case 3: FragColor *= texture(u_Textures[3], v_UV); break;
		case 4: FragColor *= texture(u_Textures[4], v_UV); break;
		case 5: FragColor *= texture(u_Textures[5], v_UV); break;
		case 6: FragColor *= texture(u_Textures[6], v_UV); break;
		case 7: FragColor *= texture(u_Textures[7], v_UV); break;
		case 8: FragColor *= texture(u_Textures[8], v_UV); break;
		case 9: FragColor *= texture(u_Textures[9], v_UV); break;
		case 10: FragColor *= texture(u_Textures[10], v_UV); break;
		case 11: FragColor *= texture(u_Textures[11], v_UV); break;
		case 12: FragColor *= texture(u_Textures[12], v_UV); break;
		case 13: FragColor *= texture(u_Textures[13], v_UV); break;
		case 14: FragColor *= texture(u_Textures[14], v_UV); break;
		case 15: FragColor *= texture(u_Textures[15], v_UV); break;
	}
}