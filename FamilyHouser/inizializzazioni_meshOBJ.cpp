#include "ShaderMaker.h"
#include "Gestione_VAO.h"
#include "geometria.h"
#include "Strutture.h"
#include "Materiali_Base.h"
#include "load_meshes_assimp.hpp"
#include "gestioneTexture.h"

extern vector<MeshObj> Model3D;
extern vector<vector<MeshObj>> ScenaObj;

extern   unsigned int programId, programId1, programIdr,programId_text, VAO_Text, VBO_Text;
Mesh Cubo, Piano, Piramide,oggetto;
extern vector<Mesh> Scena,Snowman;
extern int height, width;
extern ViewSetup SetupTelecamera;
extern PerspectiveSetup SetupProspettiva;
extern vector<Material> materials;
extern vector<Shader> shaders, illuminazioni;
extern point_light light;
extern int texture, texture1,texture2;

string Meshdir = "Meshes/";
string Imagedir = "Textures/";

void INIT_SHADER(void)

{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader_C.glsl";
	char* fragmentShader = (char*)"fragmentShader_C.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

	vertexShader = (char*)"vertexShader_CubeMap.glsl";
	fragmentShader = (char*)"fragmentShader_CubeMap.glsl";

	programId1 = ShaderMaker::createProgram(vertexShader, fragmentShader);
	//Generazione del program shader per la gestione del testo
	vertexShader = (char*)"VertexShader_Text.glsl";
	fragmentShader = (char*)"FragmentShader_Text.glsl";

	programId_text = ShaderMaker::createProgram(vertexShader, fragmentShader);
	
	vertexShader = (char*)"vertexShader_riflessione.glsl";
	fragmentShader = (char*)"fragmentShader_riflessione.glsl";

	programIdr = ShaderMaker::createProgram(vertexShader, fragmentShader);
}

void INIT_VAO(void)
{
	string name = "minnie.png";
	string path = Imagedir + name;
	texture = loadTexture(path.c_str(), 0);
	name = "muromattoni.jpg";
	path = Imagedir + name;
	texture1 = loadTexture(path.c_str(), 0);
	 
	//Skybox
	crea_cubo(&oggetto);
	crea_VAO_Vector(&oggetto);
	
	Scena.push_back(oggetto);

	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();
	 
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(-8.0, -1.0, 6.0));
	oggetto.Model = scale(oggetto.Model, vec3(0.5, 0.5, 0.5));
	oggetto.nome = "Sfera riflettente";

	oggetto.sceltaVS = ShaderOption::INTERPOLATING_SHADING;  //Interpolative Shading
	oggetto.material = MaterialType::RED_PLASTIC;
	Scena.push_back(oggetto);

	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();

	//Crea piano di appoggio
	crea_piano_suddiviso(&oggetto, vec4(0.01, 0.5451, 0.1333, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(0.0, -1.5, 0.0));
	oggetto.Model = scale(oggetto.Model, vec3(100000.0, 1.0, 100000.0));
	oggetto.nome = "Terreno ";

	oggetto.sceltaVS = ShaderOption::NONE;  // NO SHADING
	oggetto.material = MaterialType::EMERALD;
	//Scena.push_back(oggetto);

	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();
	//Crea Bandiera
	crea_cilindro(&oggetto, vec4(0.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(-8.0, -1.5, 0.0));
	oggetto.Model = scale(oggetto.Model, vec3(0.2, 6.0, 0.2));
	oggetto.nome = "Bandiera";
	oggetto.sceltaVS = ShaderOption::INTERPOLATING_SHADING;
	oggetto.material = MaterialType::SLATE;


	Scena.push_back(oggetto);

	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();
	//Crea Telo
	crea_piano_suddiviso(&oggetto, vec4(1.0, 0.0, 0.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(-6.0, 2.0, 0.0));
	oggetto.Model = scale(oggetto.Model, vec3(5.0, 1.8, 1.0));
	oggetto.Model = rotate(oggetto.Model, radians(90.0f), vec3(1.0, 0.0, 0.0));
	 
	oggetto.sceltaVS = ShaderOption::WAVE;  //Shading per bandiera sventolante
	oggetto.material = MaterialType::BRASS;
	oggetto.nome = "Telo";
	Scena.push_back(oggetto);

	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();

	//Sfera
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(4.0, 0.0, 0.0));
	oggetto.Model = scale(oggetto.Model, vec3(2.0, 2.0, 2.0));
	oggetto.nome = "Sfera";
 
	oggetto.sceltaVS = ShaderOption::PHONG_SHADING;  //Interpolative Shading
	oggetto.illuminazione = ShadingType::PHONG;
	oggetto.material = MaterialType::RED_PLASTIC;
	Scena.push_back(oggetto);
	
	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();

	//Sfera
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(10.0, 0.0, 0.0)); // Spostamento a destra con un po' di spazio
	oggetto.Model = scale(oggetto.Model, vec3(2.0, 2.0, 2.0));
	oggetto.nome = "Sfera";
	oggetto.sceltaVS = ShaderOption::INTERPOLATING_SHADING;  //Interpolative Shading
	oggetto.illuminazione = ShadingType::PHONG;
	oggetto.material = MaterialType::RED_PLASTIC;
	Scena.push_back(oggetto);

	// Pulizia delle strutture dati dell'oggetto per la prossima creazione
	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();

	//Sfera
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(4.0, -6.0, 0.0)); // Spostamento sotto rispetto alla prima sfera
	oggetto.Model = scale(oggetto.Model, vec3(2.0, 2.0, 2.0));
	oggetto.nome = "Sfera";
	oggetto.sceltaVS = ShaderOption::PHONG_SHADING;  //Interpolative Shading
	oggetto.illuminazione = ShadingType::BLINN;
	oggetto.material = MaterialType::RED_PLASTIC;
	Scena.push_back(oggetto);

	// Pulizia delle strutture dati dell'oggetto per la prossima creazione
	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();

	//Sfera
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(10.0, -6.0, 0.0)); // Spostamento sotto rispetto alla prima sfera
	oggetto.Model = scale(oggetto.Model, vec3(2.0, 2.0, 2.0));
	oggetto.nome = "Sfera";
	oggetto.sceltaVS = ShaderOption::INTERPOLATING_SHADING;  //Interpolative Shading
	oggetto.illuminazione = ShadingType::BLINN;
	oggetto.material = MaterialType::RED_PLASTIC;
	Scena.push_back(oggetto);

	// Pulizia delle strutture dati dell'oggetto per la prossima creazione
	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();


	// Creazione del parallelepipedo per il piano del tavolo
	crea_Cubo(&oggetto, vec4(1.0, 0.0, 0.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(-1.0, -3.6, 6));
	oggetto.Model = scale(oggetto.Model, vec3(4.2, 0.5, 3.2));
	oggetto.Model = rotate(oggetto.Model, radians(-180.0f), vec3(1.0, 0.0, 0.0));

	oggetto.sceltaVS = ShaderOption::PHONG_SHADING;
	oggetto.illuminazione = ShadingType::BLINN;
	oggetto.nome = "Tavolo";
	oggetto.material = MaterialType::RED_PLASTIC;
	Scena.push_back(oggetto);

	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();

	// Creazione delle gambe del tavolo
	Mesh gamba1, gamba2, gamba3, gamba4;

	// Scala e traslazione per la gamba 1
	mat4 scala_gamba1 = scale(mat4(1.0), vec3(0.1, 2.0, 0.1));
	mat4 traslazione_gamba1 = translate(mat4(1.0), vec3(-1.8, -4.1, 5.2));
	gamba1.Model = traslazione_gamba1 * scala_gamba1;

	// Scala e traslazione per la gamba 2
	mat4 scala_gamba2 = scale(mat4(1.0), vec3(0.1, 2.0, 0.1));
	mat4 traslazione_gamba2 = translate(mat4(1.0), vec3(-1.8, -4.1, 6.8));
	gamba2.Model = traslazione_gamba2 * scala_gamba2;

	// Scala e traslazione per la gamba 3
	mat4 scala_gamba3 = scale(mat4(1.0), vec3(0.1, 2.0, 0.1));
	mat4 traslazione_gamba3 = translate(mat4(1.0), vec3(1.8, -4.1, 5.2));
	gamba3.Model = traslazione_gamba3 * scala_gamba3;

	// Scala e traslazione per la gamba 4
	mat4 scala_gamba4 = scale(mat4(1.0), vec3(0.1, 2.0, 0.1));
	mat4 traslazione_gamba4 = translate(mat4(1.0), vec3(1.8, -4.1, 6.8));
	gamba4.Model = traslazione_gamba4 * scala_gamba4;

	// Creazione delle gambe come cubi e aggiunta alla scena
	crea_Cubo(&gamba1, vec4(1.0, 0.0, 0.0, 1.0));
	crea_Cubo(&gamba2, vec4(1.0, 0.0, 0.0, 1.0));
	crea_Cubo(&gamba3, vec4(1.0, 0.0, 0.0, 1.0));
	crea_Cubo(&gamba4, vec4(1.0, 0.0, 0.0, 1.0));

	// Creazione del Vertex Array Object (VAO) per ogni gamba
	crea_VAO_Vector(&gamba1);
	crea_VAO_Vector(&gamba2);
	crea_VAO_Vector(&gamba3);
	crea_VAO_Vector(&gamba4);

	gamba1.sceltaVS = ShaderOption::PHONG_SHADING;
	gamba1.illuminazione = ShadingType::BLINN;
	gamba1.nome = "Gamba";
	gamba1.material = MaterialType::RED_PLASTIC;

	gamba2.sceltaVS = ShaderOption::PHONG_SHADING;
	gamba2.illuminazione = ShadingType::BLINN;
	gamba2.nome = "Gamba";
	gamba2.material = MaterialType::RED_PLASTIC;

	gamba3.sceltaVS = ShaderOption::PHONG_SHADING;
	gamba3.illuminazione = ShadingType::BLINN;
	gamba3.nome = "Gamba";
	gamba3.material = MaterialType::RED_PLASTIC;

	gamba4.sceltaVS = ShaderOption::PHONG_SHADING;
	gamba4.illuminazione = ShadingType::BLINN;
	gamba4.nome = "Gamba";
	gamba4.material = MaterialType::RED_PLASTIC;

	// Aggiunta delle gambe alla scena
	Scena.push_back(gamba1);
	Scena.push_back(gamba2);
	Scena.push_back(gamba3);
	Scena.push_back(gamba4);


	//
	//bool obj;
	//name = "Shine_Sprite.obj";
	//path = Meshdir + name;
	//obj = loadAssImp(path.c_str(), Model3D);

 //	printf("Model3D %d \n", Model3D.size());

	//int nmeshes = Model3D.size();

	//for (int i = 0; i < nmeshes; i++)
	//{
	//	crea_VAO_Vector_MeshObj(&Model3D[i]);
	//	Model3D[i].ModelM = mat4(1.0);
	//	Model3D[i].ModelM = translate(Model3D[i].ModelM, vec3(2.0, 6.0, 12.0));
	//	Model3D[i].ModelM = scale(Model3D[i].ModelM, vec3(0.5, 0.5,0.5));
	//	Model3D[i].nome = "Moneta";
	//	Model3D[i].sceltaVS = ShaderOption::NO_TEXTURE;

	// 
	//	vec3 ambiental = Model3D[i].materiale.ambient;
	//	vec3 diffusivo = Model3D[i].materiale.diffuse;
	//	vec3 speculare = Model3D[i].materiale.specular;
	//	Model3D[i].materiale.ambient = ambiental;
	//	Model3D[i].materiale.diffuse = diffusivo;
	//	Model3D[i].materiale.specular = speculare;
	//	Model3D[i].materiale.shininess = red_plastic_shininess;

	//}

	//ScenaObj.push_back(Model3D);

	//Model3D.clear();

	//name = "house.obj";
	//path = Meshdir + name;
	//obj = loadAssImp(path.c_str(), Model3D);

	//printf("Model3D %d \n", Model3D.size());

	//nmeshes = Model3D.size();

	//for (int i = 0; i < nmeshes; i++)
	//{
	//	crea_VAO_Vector_MeshObj(&Model3D[i]);
	//	Model3D[i].ModelM = mat4(1.0);
	//	Model3D[i].ModelM = translate(Model3D[i].ModelM, vec3(2.0, -1.0, 16.0));
	//	Model3D[i].ModelM = scale(Model3D[i].ModelM, vec3(0.5, 0.5, 0.5));
	//	Model3D[i].nome = "POkemon";
	//	Model3D[i].sceltaVS = ShaderOption::NO_TEXTURE;


	//	vec3 ambiental = Model3D[i].materiale.ambient;
	//	vec3 diffusivo = Model3D[i].materiale.diffuse;
	//	vec3 speculare = Model3D[i].materiale.specular;
	//	Model3D[i].materiale.ambient = ambiental;
	//	Model3D[i].materiale.diffuse = diffusivo;
	//	Model3D[i].materiale.specular = speculare;
	//	Model3D[i].materiale.shininess = red_plastic_shininess;

	//}

	//ScenaObj.push_back(Model3D);
	
	 

}
 

void INIT_VAO_Text(void)
{

	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO_Text);
	glGenBuffers(1, &VBO_Text);
	glBindVertexArray(VAO_Text);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Text);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void INIT_CAMERA_PROJECTION(void)
{
	//Imposto la telecamera
 
	SetupTelecamera.position = glm::vec4(0.0, 0.5, 20.0, 0.0);
	SetupTelecamera.target = glm::vec4(0.0, 0.0, 0.0, 0.0);
	SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;
	SetupTelecamera.upVector = glm::vec4(0.0, 1.0, 0.0, 0.0);


	//Imposto la proiezione prospettica
	 
	SetupProspettiva.aspect = (GLfloat)width / (GLfloat)height;
	SetupProspettiva.fovY = 45.0f;
	SetupProspettiva.far_plane = 2000.0f;
	SetupProspettiva.near_plane = 0.1f;
}

void INIT_Illuminazione()
{
	light.position = glm::vec4(0.0, 5.5, 20.0, -2.0);
	light.color = { 1.0, 1.0, 1.0 };
	light.power = 1.f;

	//Setup dei materiali
	// Materials setup
	materials.resize(6);
	materials[MaterialType::RED_PLASTIC].name = "Red Plastic";
	materials[MaterialType::RED_PLASTIC].ambient = red_plastic_ambient;
	materials[MaterialType::RED_PLASTIC].diffuse = red_plastic_diffuse;
	materials[MaterialType::RED_PLASTIC].specular = red_plastic_specular;
	materials[MaterialType::RED_PLASTIC].shininess = red_plastic_shininess;

	materials[MaterialType::EMERALD].name = "Emerald";
	materials[MaterialType::EMERALD].ambient = emerald_ambient;
	materials[MaterialType::EMERALD].diffuse = emerald_diffuse;
	materials[MaterialType::EMERALD].specular = emerald_specular;
	materials[MaterialType::EMERALD].shininess = emerald_shininess;

	materials[MaterialType::BRASS].name = "Brass";
	materials[MaterialType::BRASS].ambient = brass_ambient;
	materials[MaterialType::BRASS].diffuse = brass_diffuse;
	materials[MaterialType::BRASS].specular = brass_specular;
	materials[MaterialType::BRASS].shininess = brass_shininess;

	materials[MaterialType::SLATE].name = "Slate";
	materials[MaterialType::SLATE].ambient = slate_ambient;
	materials[MaterialType::SLATE].diffuse = slate_diffuse;
	materials[MaterialType::SLATE].specular = slate_specular;
	materials[MaterialType::SLATE].shininess = slate_shininess;

	materials[MaterialType::YELLOW].name = "Yellow";
	materials[MaterialType::YELLOW].ambient = yellow_ambient;
	materials[MaterialType::YELLOW].diffuse = yellow_diffuse;
	materials[MaterialType::YELLOW].specular = yellow_specular;
	materials[MaterialType::YELLOW].shininess = yellow_shininess;


	materials[MaterialType::NO_MATERIAL].name = "NO_MATERIAL";
	materials[MaterialType::NO_MATERIAL].ambient = glm::vec3(1, 1, 1);
	materials[MaterialType::NO_MATERIAL].diffuse = glm::vec3(0, 0, 0);
	materials[MaterialType::NO_MATERIAL].specular = glm::vec3(0, 0, 0);
	materials[MaterialType::NO_MATERIAL].shininess = 1.f;

	//Setup degli shader
	shaders.resize(5);
	shaders[ShaderOption::NONE].value = 0;
	shaders[ShaderOption::NONE].name = "NONE";
	shaders[ShaderOption::INTERPOLATING_SHADING].value = 1;
	shaders[ShaderOption::INTERPOLATING_SHADING].name = "INTERPOLATING SHADING";
	shaders[ShaderOption::PHONG_SHADING].value = 2;
	shaders[ShaderOption::PHONG_SHADING].name = "PHONG SHADING";
	shaders[ShaderOption::NO_TEXTURE].value = 3;
	shaders[ShaderOption::NO_TEXTURE].name = "NO TEXTURE";
	shaders[ShaderOption::WAVE].value = 4;
	shaders[ShaderOption::WAVE].name = "WAVE";

	//setup illuminazioni
	illuminazioni.resize(3);
	illuminazioni[ShadingType::NO_NONE].value = 0;
	illuminazioni[ShadingType::NO_NONE].name = "NO_NONE";
	illuminazioni[ShadingType::PHONG].value = 1;
	illuminazioni[ShadingType::PHONG].name = "PHONG";
	illuminazioni[ShadingType::BLINN].value = 2;
	illuminazioni[ShadingType::BLINN].name = "BLINN";
}
