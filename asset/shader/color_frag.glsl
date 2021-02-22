#version 330 core

in vec2  v_UV;
in vec4  v_Color;
in float v_Texure;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

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
		case 16: FragColor *= texture(u_Textures[16], v_UV); break;
		case 17: FragColor *= texture(u_Textures[17], v_UV); break;
		case 18: FragColor *= texture(u_Textures[18], v_UV); break;
		case 19: FragColor *= texture(u_Textures[19], v_UV); break;
		case 20: FragColor *= texture(u_Textures[20], v_UV); break;
		case 21: FragColor *= texture(u_Textures[21], v_UV); break;
		case 22: FragColor *= texture(u_Textures[22], v_UV); break;
		case 23: FragColor *= texture(u_Textures[23], v_UV); break;
		case 24: FragColor *= texture(u_Textures[24], v_UV); break;
		case 25: FragColor *= texture(u_Textures[25], v_UV); break;
		case 26: FragColor *= texture(u_Textures[26], v_UV); break;
		case 27: FragColor *= texture(u_Textures[27], v_UV); break;
		case 28: FragColor *= texture(u_Textures[28], v_UV); break;
		case 29: FragColor *= texture(u_Textures[29], v_UV); break;
		case 30: FragColor *= texture(u_Textures[30], v_UV); break;
		case 31: FragColor *= texture(u_Textures[31], v_UV); break;
	}
}