#version 330 core

in vec2 UV;
in vec4 Color;
in float Tex;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

void main()
{
    FragColor = Color;

    switch(int(Tex))
	{
		case 0: FragColor *= texture(u_Textures[0], UV); break;
		case 1: FragColor *= texture(u_Textures[1], UV); break;
		case 2: FragColor *= texture(u_Textures[2], UV); break;
		case 3: FragColor *= texture(u_Textures[3], UV); break;
		case 4: FragColor *= texture(u_Textures[4], UV); break;
		case 5: FragColor *= texture(u_Textures[5], UV); break;
		case 6: FragColor *= texture(u_Textures[6], UV); break;
		case 7: FragColor *= texture(u_Textures[7], UV); break;
		case 8: FragColor *= texture(u_Textures[8], UV); break;
		case 9: FragColor *= texture(u_Textures[9], UV); break;
		case 10: FragColor *= texture(u_Textures[10], UV); break;
		case 11: FragColor *= texture(u_Textures[11], UV); break;
		case 12: FragColor *= texture(u_Textures[12], UV); break;
		case 13: FragColor *= texture(u_Textures[13], UV); break;
		case 14: FragColor *= texture(u_Textures[14], UV); break;
		case 15: FragColor *= texture(u_Textures[15], UV); break;
		case 16: FragColor *= texture(u_Textures[16], UV); break;
		case 17: FragColor *= texture(u_Textures[17], UV); break;
		case 18: FragColor *= texture(u_Textures[18], UV); break;
		case 19: FragColor *= texture(u_Textures[19], UV); break;
		case 20: FragColor *= texture(u_Textures[20], UV); break;
		case 21: FragColor *= texture(u_Textures[21], UV); break;
		case 22: FragColor *= texture(u_Textures[22], UV); break;
		case 23: FragColor *= texture(u_Textures[23], UV); break;
		case 24: FragColor *= texture(u_Textures[24], UV); break;
		case 25: FragColor *= texture(u_Textures[25], UV); break;
		case 26: FragColor *= texture(u_Textures[26], UV); break;
		case 27: FragColor *= texture(u_Textures[27], UV); break;
		case 28: FragColor *= texture(u_Textures[28], UV); break;
		case 29: FragColor *= texture(u_Textures[29], UV); break;
		case 30: FragColor *= texture(u_Textures[30], UV); break;
		case 31: FragColor *= texture(u_Textures[31], UV); break;
	}
}