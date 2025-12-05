#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <string>

//project sample for LAUC project

Vector2 MOUSEPOSITION;              //Mouse position
Vector2 PLAYER_POS;                 //player [ball] position

Sound GUN_SHOT;                     //gun shot sound
Sound ZOMBIE;                       //zombie sound

const int Screen_H = 800;           //Screen Height
const int Screen_W = 1000;          //Screen Width
static int score = 0;               //score of player
static float game_time = 0;         //timer for increasing speed
static float Health = 100.0f;       //player health
static float Max_Health = 100.0f;   //maximum player health
static float fire_rate = 0;         //controls fire rate of bullet
static float bullet_speed = 10.0f;  //speed of bullets
static float bg_size = 3.0f;

float player_velocity = 0;          //player jumping velo
float player_gravity = 0.5f;        //gravity affecting player
float player_jumpforce = -16;       //jympforce of player
int player_groundLevel = 660;       //ground level
bool is_PLAYER_jumping = false;   
int player_dir = 1;                 //which direction player is facing [1 for right and -1 for left]

const int MAX_BULLET = 500;         //player has 500 bullets for array
int Player_Bullets = 250;           //player has 150 bulltes initially
const int MAX_ENEMY = 50;           //50 enemies for array

int currentFrame = 0;               //frmae count
int currentFrame_player = 0;
float Frame_delay = 0.24f;          //delay for frame
float Frame_time = 0;               //frame time

float Frame_delay_player = 0.24f;    //delay for frame
float Frame_time_player = 0;        //frame time

int selected_option = 0;            //which option is selected to change color
float timer;                        //timer 

//ENEMY ANIAMTION INITIALIZE for death
const int ENEMY_RUNNING = 19;       //enemy frames for running
const int IDLE = 4;                 //player idle animation

//camara following player
Camera2D camara = { 0 };

Texture2D PLAYER[IDLE];
Texture2D BULLET;
Texture2D ENEMY_RUN[ENEMY_RUNNING];
Texture2D BACKGROUND;
Texture2D BACKGROUND_SKY;

//multiple bullets
struct Bullet {
	Vector2 POS;
	Vector2 VEL;
	bool Is_BULLET_active = false;
};


//multiple enemies
struct Enemy {
	Vector2 POS;
	Vector2 VEL;
	bool Is_ENEMY_active = false;
	int hp = 100;
	int Max_hp = 100;
};


//Check option
struct CORRECT {
	int is_OPTION_correct = -1;
	bool is_OPTION_selected = false;
	bool one_option = false;
	bool Next_Stage = false;
};


//struct declaration
Bullet NUM_BULLET[MAX_BULLET];             
Enemy NUM_ENEMY[MAX_ENEMY];
CORRECT INDEX[20];


//game states
enum State { MENU, GAME ,CREDITS, PAUSE, GAME_OVER , WAVE1 , WAVE2 , WAVE3 , WAVE4 , END , Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10 , Q11, Q12, Q13, Q14, Q15, Q16, Q17, Q18, Q19, Q20 , EXIT };
State CurrentState = MENU;


//------------------------Questions and their options----------//
const char* Question[1000] = { "Q1] Whats the capital of Russia" ,
							   "Q2] Whats the symbol of Iridium" ,
							   "Q3] SOP full form" ,
							   "Q4] Who invented the Number ZERO" ,
							   "Q5] Who painted the Mona Lisa" ,
							   "Q6] Which year WW1 ended" ,
							   "Q7] Which of these isnt Radioactive" ,
							   "Q8] which of these is not a Carbon Allotropy" ,
							   "Q9] How many different states does a 3-bit \n asynchronous down counter have?",
							   "Q10] In a combinational circuit, the \n output at any time depends only on the \n _______ at that time." ,

							   "Q11] Hardness of Water should be ____ \nfor Industiral use" ,
							   "Q12] Regen of Anion exchange \nresin is done by " ,
							   "Q13] Who gave idea of General Relativity " ,
							   "Q14] The Color of M-EBT Complex is " ,
							   "Q15] In Proximate Analysis \n, which is not detected" ,
							   "Q16] Whats the integration of sin(x)" ,
							   "Q17] Main Constitue of CNG is" ,
							   "Q18] Whats 2+(2(2) + 2)/2" ,
							   "Q19] The Instrument Used to determine Current is called" ,
							   "Q20] Who discovered Gravity"
};


const char* Options[1000] = { "Moscow" , "Coscow" , "Ethiopia" , "Delhi" ,                                                  //1 Ans : A
							  "I" , "Id" , "Ir" , "Dr" ,                                                                    //2 Ans : C
							  "Standard Optimization pill" , "Standard Operating Procedure" , "Sum Of Products" , "Standard Operation Protocol" , //3 Ans : C
							  "Newton" , "Aryabhatta" , "Pythagorus" , "Apollo" ,                              //4 Ans : B
							  "Sandro Botticelli" , "Edvard Munch" , "Vincent van Gogh" , "Leonardo da Vinci" ,//5 Ans : D
							  "1900" , "1893" , "1918" , "1945" ,                                              //6 Ans : C
							  "Curium" , "Cadmium" , "Uranium" , "Plutonium" ,                                 //7 Ans : B
							  "Graphite" , "alpha-rhombohedral" , "buckminsterfullerene " , "Diamond" ,            //8 Ans : B
							  "2" , "4" , "6" , "8" ,                                                          //9 Ans : D
							  "Voltage" , "Intermediate values" , "Input values" , "Clock pulses" ,            //10 Ans : C

							  "100 - 200 ppm" , "200 - 300 ppm" , "Above 300ppm" , "Below 10 ppm" ,            //11Ans : D
							  "Dil Na2SO4" , "Dil HCl" , "Dil NaOH" , "NaCl" ,                                 //12Ans : C
							  "Albert Einstein" , "Nikola Tesla" , "Stephan Hawking" , "Thomson" ,             //13Ans : A
							  "Wine Red" , "Blue" , "Colorless" , "Pink" ,                                     //14Ans : A
							  "Ash" , "FC" , "Sulphur" , "Moisture" ,                                          //15Ans : C
							  "2.sin(x).cos(x)" , "-sin(x)" , "-cos(x)" , "cos(x)" ,                           //16Ans : C
							  "Methane" , "Ethylene" , "Butane" , "Pentane"  ,                                 //17Ans : A
							  "8" , "6" , "5" , "4" ,                                                          //18Ans : C
							  "Calorimeter" , "Ammeter" , "Viscometer" , "Conductometer" ,                     //19Ans : B
							  "Galio" , "Einstein" , "Thomson" , "Newton"                                      //20Ans : D
};

//-------------------------------------------------------------//


//-------------------------------options-----------------------//
Rectangle O1 = { 100 , 300 , 500 , 60 };
Rectangle O2 = { 100 , 400 , 500 , 60 };
Rectangle O3 = { 100 , 500 , 500 , 60 };
Rectangle O4 = { 100 , 600 , 500 , 60 };
Color CO1;
Color CO2;
Color CO3;
Color CO4;
//-------------------------------------------------------------//


//astetic function and other
void static BORDER_FUNCTION();              //Border around corner
void RESET_BASIC();                         //resets everything to its initialize state
void RESET_ENEMY_BULLET();                  //resets enemies and bullets
void IMAGES_SOUND();                        //loads images and sound
void ENEMY_ANIMATION_RUN();                 //loads animation of enemy running
void ENEMY_ANIMATION_PLAY();                //plays animation of enemy
void BACKGROUND_SCENARIO();                 //draws the background
void PLAYER_IDLE();
void PLAY_PLAYER_IDLE();

//functions creating buttons
void MAKE_BUTTON(Rectangle Button, Color Normal, Color Hovered, const char* Name, int Offset_X, int Offset_Y, int Size, State NewState , bool reset = true) {
	bool Hover = CheckCollisionPointRec(MOUSEPOSITION, Button);
	DrawRectangleRec(Button, Hover ? Normal : Hovered);
	DrawText(Name, Button.x + Offset_X, Button.y + Offset_Y, Size, BLACK);

	if (Hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		CurrentState = NewState;
		if(reset)RESET_BASIC();
		reset = true;
	}
}
void RETURN_FUNCTION();                     //return button
void UI_FUNCTION();                         //UI buttons
void BACK_FUNCTION();                       //back to menu function if game is paused
void PLAY_AGAIN();                          //play again function
void CONTINUE_FUNCTION();                   //continue playing
void PAUSE_FUNCTION();                      //pause menu


//functions related to player
void static PLAYER_BALL_FUNCTION();         //player movement
void static PROJECTILES_FUNCTION();         //players projectile
void HEALTH();                              //checking if player Health is 100
void JUMP();                                //player "JUMPS"!


//functions related to enemies
void static SPWAN_ENEMIES_FUNCTION();       //making enemies
void static UPDATE_ENEMY_FUNCTION();        //spwaning enemy
void static CHECK_HIT_ENEMY_FUNCTION();     //checking if bullet hit the enemy
void static SPEED(int speed_rate);          //speed of enemies
void static TIME_SPEED_FUNCTION();          //determies speed of enemies based on the wave
void HEALTH_BAR_ENEMY();                    //health bar of enemy


//in game functions
void PLAYER_HIT_FUNCTION();                 //check if rectangle
void START_OF_GAME();                       //game start 
void WAVE_TEXT(int wave_number);            //starts playing wave animation[text]


//functions related to shop
void SHOPING();                             //opens shop depending on the wave
void NEXT(State NextState);                 //takes user to next state
void OPTIONS();                             //making option
void OPTION_BUTTON();                       //making rectangles for these options
void QUESTION1();                           //first question
void QUESTION2();                           //second question
void QUESTION3();                           //third question
void QUESTION4();                           //forth question
void QUESTION5();                           //fifth question
void QUESTION6();                           //sixth question
void QUESTION7();                           //Seventh question
void QUESTION8();                           //Eight question
void QUESTION9();                           //ninth question
void QUESTION10();                          //tenth question
void QUESTION11();                          //11th question
void QUESTION12();                          //12th question
void QUESTION13();                          //13th question
void QUESTION14();                          //14th question
void QUESTION15();                          //15th question
void QUESTION16();                          //16th question
void QUESTION17();                          //17th question
void QUESTION18();                          //18th question
void QUESTION19();                          //19th question
void QUESTION20();                          //20th question


//after game function
void static GAME_OVER_FUNCTION();           //game over and back to menu button
void END_OF_GAME();                         //end of game


//start of main function
int main() {
	InitWindow(Screen_W, Screen_H, "PROJECT");
	InitAudioDevice();
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	PLAYER_POS = { 500 , 660 };                   //player position

	//camara following player
	camara.offset = {Screen_W/2.0f , Screen_H/2.0f};
	camara.rotation = 0.0f;
	camara.zoom = 1.0f;

	IMAGES_SOUND();
	ENEMY_ANIMATION_RUN();
	PLAYER_IDLE();

	while (!WindowShouldClose()) {
		MOUSEPOSITION = GetMousePosition();

		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		if (CurrentState == MENU) {

			DrawText("ZomShoot", 370, 100, 70, RED);
			BORDER_FUNCTION();
			UI_FUNCTION();
			DrawText("Movements" , 700 , 650 , 20 , BLACK);
			DrawText("Mouse-Left-Click : Fires bullet left", 590, 680, 20, BLACK);
			DrawText("Mouse-Right-Click : Fires bullet right", 590, 710, 20, BLACK);
			DrawText("Space : Jump", 590, 740, 20, BLACK);

		}
	
//========================================================//
		else if (CurrentState == WAVE1) {
			START_OF_GAME();
			WAVE_TEXT(1);
			
			if (score >= 100) {
				CurrentState = WAVE2;
				RESET_ENEMY_BULLET();
			}
		}

		else if (CurrentState == WAVE2) {
			START_OF_GAME();
			WAVE_TEXT(2);

			if (score >= 200) {
				CurrentState = WAVE3;
				RESET_ENEMY_BULLET();
			}
		}

		else if (CurrentState == WAVE3) {
			START_OF_GAME();
			WAVE_TEXT(3);

			if (score >= 300) {
				CurrentState = WAVE4;
			}
		}

		else if (CurrentState == WAVE4) {
			START_OF_GAME();
			WAVE_TEXT(4);
			if (score >= 400) {
				CurrentState = END;
			}
		}
//========================================================//

//========================================================//
		else if (CurrentState == Q1) {
			QUESTION1();
		}

		else if (CurrentState == Q2) {
			QUESTION2();
		}

		else if (CurrentState == Q3) {
			QUESTION3();
		}

		else if (CurrentState == Q4) {
			QUESTION4();
		}

		else if (CurrentState == Q5) {
			QUESTION5();
		}

		else if (CurrentState == Q6) {
			QUESTION6();
		}

		else if (CurrentState == Q7) {
			QUESTION7();
		}

		else if (CurrentState == Q8) {
			QUESTION8();
		}

		else if (CurrentState == Q9) {
			QUESTION9();
		}

		else if (CurrentState == Q10) {
			QUESTION10();
		}

		else if (CurrentState == Q11) {
			QUESTION11();
		}

		else if (CurrentState == Q12) {
			QUESTION12();
		}

		else if (CurrentState == Q13) {
			QUESTION13();
		}

		else if (CurrentState == Q14) {
			QUESTION14();
		}

		else if (CurrentState == Q15) {
			QUESTION15();
			}

		else if (CurrentState == Q16) {
				QUESTION16();
				}

		else if (CurrentState == Q17) {
			QUESTION17();
		}

		else if (CurrentState == Q18) {
			QUESTION18();
			}

		else if (CurrentState == Q19) {
			QUESTION19();
		}

		else if (CurrentState == Q20) {
			QUESTION20();
		}

//========================================================//

//========================================================//
		else if (CurrentState == CREDITS) {
			DrawText("Made By : Primo", 100, 100, 50, RED);
			DrawText("Resources : Raylib Library", 100, 200, 50, RED);
			RETURN_FUNCTION();
		}

		else if (CurrentState == GAME_OVER) {
			GAME_OVER_FUNCTION();
		}

		else if (CurrentState == END) {
			END_OF_GAME();
		}

		else if (CurrentState == PAUSE) {
			PAUSE_FUNCTION();
		}

		else if (CurrentState == EXIT) {
			break;
		}
//========================================================//

		EndDrawing();
	}

	for (int i = 0; i < ENEMY_RUNNING; i++)
	{
		UnloadTexture(ENEMY_RUN[i]);
	}

	UnloadSound(GUN_SHOT);
	for (int i = 0; i < IDLE; i++) {
		UnloadTexture(PLAYER[i]);
	}
	CloseWindow();
	return 0;
}


//Border around corner
void static BORDER_FUNCTION() {

	//lines with thickness
	Vector2 L1_S = { 0 , 10 };
	Vector2 L1_E = { 1000 , 10 };
	Vector2 L2_S = { 10 , 0 };
	Vector2 L2_E = { 10 , 800 };
	Vector2 L3_S = { 990 , 0 };
	Vector2 L3_E = { 990 , 800 };
	Vector2 L4_S = { 0 , 790 };
	Vector2 L4_E = { 1000 , 790 };

	//drawing these lines
	DrawLineEx(L1_S, L1_E, 30, BLACK);
	DrawLineEx(L2_S, L2_E, 30, BLACK);
	DrawLineEx(L3_S, L3_E, 30, BLACK);
	DrawLineEx(L4_S, L4_E, 30, BLACK);
}


//loads images and sound
void IMAGES_SOUND() {
	BULLET = LoadTexture("Assets/All_Fire_Bullet_Pixel_16x16_00.png");
	BACKGROUND = LoadTexture("Assets/BG.png");
	BACKGROUND_SKY = LoadTexture("Assets/BGSKY.png.png");

	SetTextureFilter(BULLET, TEXTURE_FILTER_POINT);
	SetTextureFilter(BACKGROUND, TEXTURE_FILTER_POINT);
	SetTextureFilter(BACKGROUND_SKY, TEXTURE_FILTER_POINT);

	GUN_SHOT = LoadSound("Assets/mixkit-game-gun-shot-1662.mp3");
	ZOMBIE = LoadSound("Assets/minecraft-zombie-hit.mp3");
}


//enemy running animation
void ENEMY_ANIMATION_RUN() {
	ENEMY_RUN[0] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-1.png.png");
	ENEMY_RUN[1] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-2.png.png");
	ENEMY_RUN[2] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-3.png.png");
	ENEMY_RUN[3] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-4.png.png");
	ENEMY_RUN[4] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-5.png.png");
	ENEMY_RUN[5] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-6.png.png");
	ENEMY_RUN[6] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-7.png.png");
	ENEMY_RUN[7] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-8.png.png");
	ENEMY_RUN[8] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-9.png.png");
	ENEMY_RUN[9] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-16.png.png");
	ENEMY_RUN[10] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-17.png.png");
	ENEMY_RUN[11] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-18.png.png");
	ENEMY_RUN[12] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-19.png.png");
	ENEMY_RUN[13] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-20.png.png");
	ENEMY_RUN[14] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-21.png.png");
	ENEMY_RUN[15] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-22.png.png");
	ENEMY_RUN[16] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-23.png.png");
	ENEMY_RUN[17] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-24.png.png");
	ENEMY_RUN[18] = LoadTexture("Assets/ZOMBIE_RUN/running_zom-25.png.png");

	for (int i = 0; i < 18; i++) {
		SetTextureFilter(ENEMY_RUN[i], TEXTURE_FILTER_POINT);
	}
}


//plays enemy animation
void ENEMY_ANIMATION_PLAY() {
	Frame_time += GetFrameTime();

	if (Frame_time > Frame_delay) {
		currentFrame = (currentFrame + 1) % ENEMY_RUNNING;
		Frame_time = 0;
	}
}


//player idle
void PLAYER_IDLE() {
	PLAYER[0] = LoadTexture("Assets/PLAYER_IDLE/Gunner_Idle_Preview-1.png.png");
	PLAYER[1] = LoadTexture("Assets/PLAYER_IDLE/Gunner_Idle_Preview-2.png.png");
	PLAYER[2] = LoadTexture("Assets/PLAYER_IDLE/Gunner_Idle_Preview-3.png.png");
	PLAYER[3] = LoadTexture("Assets/PLAYER_IDLE/Gunner_Idle_Preview-4.png.png");

	for (int i = 0; i < 4; i++) {
		SetTextureFilter(PLAYER[i], TEXTURE_FILTER_POINT);
	}
}


//play player animation
void PLAY_PLAYER_IDLE() {
	Frame_time_player += GetFrameTime();

	if (Frame_time_player > Frame_delay_player) {
		currentFrame_player = (currentFrame_player + 1) % IDLE;
		Frame_time_player = 0;
	}
}


//draws the background
void BACKGROUND_SCENARIO() {
	//=======================GROUND============================================//
	DrawTexturePro(BACKGROUND,
		Rectangle{ 0 , 0 , (float)BACKGROUND.width , (float)BACKGROUND.height },
		Rectangle{ 0, 260, (float)Screen_W, (float)Screen_H },
		Vector2{ -1000 , 0 }, 0, WHITE);
	DrawTexturePro(BACKGROUND,
		Rectangle{ 0 , 0 , (float)BACKGROUND.width , (float)BACKGROUND.height },
		Rectangle{ 0, 260, (float)Screen_W, (float)Screen_H },
		Vector2{ 0 , 0 }, 0, WHITE);
	DrawTexturePro(BACKGROUND,
		Rectangle{ 0 , 0 , (float)BACKGROUND.width , (float)BACKGROUND.height },
		Rectangle{ 0, 260, (float)Screen_W, (float)Screen_H },
		Vector2{ 1000 , 0 }, 0, WHITE);
	DrawTexturePro(BACKGROUND,
		Rectangle{ 0 , 0 , (float)BACKGROUND.width , (float)BACKGROUND.height },
		Rectangle{ 0, 260, (float)Screen_W, (float)Screen_H },
		Vector2{ 2000 , 0 }, 0, WHITE);
	//===========================SKY==============================================//
	DrawTexturePro(BACKGROUND_SKY,
		Rectangle{ 0 , 0 , (float)BACKGROUND_SKY.width , (float)BACKGROUND_SKY.height },
		Rectangle{ 0, 260, (float)Screen_W, (float)Screen_H },
		Vector2{ 0 , 800 }, 0, WHITE);
	DrawTexturePro(BACKGROUND_SKY,
		Rectangle{ 0 , 0 , (float)BACKGROUND_SKY.width , (float)BACKGROUND_SKY.height },
		Rectangle{ 0, 260, (float)Screen_W, (float)Screen_H },
		Vector2{ -1000 , 800 }, 0, WHITE);
	DrawTexturePro(BACKGROUND_SKY,
		Rectangle{ 0 , 0 , (float)BACKGROUND_SKY.width , (float)BACKGROUND_SKY.height },
		Rectangle{ 0, 260, (float)Screen_W, (float)Screen_H },
		Vector2{ 2000 , 800 }, 0, WHITE);
	DrawTexturePro(BACKGROUND_SKY,
		Rectangle{ 0 , 0 , (float)BACKGROUND_SKY.width , (float)BACKGROUND_SKY.height },
		Rectangle{ 0, 260, (float)Screen_W, (float)Screen_H },
		Vector2{ 1000 , 800 }, 0, WHITE);

}


//checking if player Health is 100
void HEALTH() {
	Rectangle Player_Health_Bar = { 380 , 15 , 100 , 25 };

     	float BarX = Player_Health_Bar.x;
		float BarY = Player_Health_Bar.y ;
		float BarHeight = Player_Health_Bar.height;
		float BarWidth = 250;

		DrawRectangle(BarX , BarY , BarWidth , BarHeight , LIGHTGRAY);

		float Ratio = (float)Health / (float)Max_Health;

		DrawRectangle(BarX, BarY, BarWidth * Ratio, BarHeight, GREEN);

	if (Health <= 0) {
		CurrentState = GAME_OVER;
	}
}


//resets everything to its initialize state
void RESET_BASIC() {
	score = 0;
	game_time = 0;
	Health = 100;
	Max_Health = 100;
	bullet_speed = 10.0f;
	RESET_ENEMY_BULLET();
	for (int i = 0; i < MAX_ENEMY; i++) {
		NUM_ENEMY[i].hp = NUM_ENEMY[i].Max_hp = 100;
	}
	Player_Bullets = 250;
}


//resets enemies and bullets
void RESET_ENEMY_BULLET() {
	for (int i = 0; i < MAX_BULLET; i++) {
		NUM_BULLET[i].Is_BULLET_active = false;
	}

	for (int i = 0; i < MAX_ENEMY; i++) {
		NUM_ENEMY[i].Is_ENEMY_active = false;
		NUM_ENEMY[i].hp = 100;
	    NUM_ENEMY[i].Max_hp = 100;
	}
}


//return button
void RETURN_FUNCTION() {
	if (IsKeyPressed(KEY_ESCAPE)) {
		CurrentState = MENU;
		RESET_BASIC();
	}
}


//UI button
void UI_FUNCTION() {
	MAKE_BUTTON({ 380 , 300 , 270 , 70 }, GRAY, GREEN, "START", 30, 10, 50, WAVE1 , true);
	MAKE_BUTTON({ 380 , 400 , 270 , 70 }, GRAY, GOLD, "CREDITS", 30, 10, 50, CREDITS , true);
	MAKE_BUTTON({ 380 , 500 , 270 , 70 }, GRAY, RED, "EXIT", 30, 10, 50, EXIT , true);
}


//back to menu function if game is paused
void BACK_FUNCTION() {
	MAKE_BUTTON({ 370 ,600 , 200 , 70 }, GRAY, GREEN, "BACK", 30, 10, 50, MENU , true);
}


//play again function
void PLAY_AGAIN() {
	MAKE_BUTTON({ 300 ,500 , 350 , 70 }, GRAY, GREEN, "PLAY AGAIN", 30, 10, 50, WAVE1 , true);
}


//continue playing
void CONTINUE_FUNCTION() {
	MAKE_BUTTON({ 300 ,500 , 350 , 70 }, GRAY, GREEN, "CONTINUE", 35, 15, 50, WAVE1, false);
}


//pause menu
void PAUSE_FUNCTION() {

	DrawText(TextFormat("CURRENT SCORE : %d", score), 250, 350, 50, RED);
	BACK_FUNCTION();
	CONTINUE_FUNCTION();
	BORDER_FUNCTION();

}


//player jumps
void JUMP() {
	if (IsKeyPressed(KEY_SPACE) && !is_PLAYER_jumping) {
		player_velocity = player_jumpforce;
		is_PLAYER_jumping = true;
	}

	player_velocity += player_gravity;
	PLAYER_POS.y += player_velocity;

	if (PLAYER_POS.y > player_groundLevel) {
		PLAYER_POS.y = player_groundLevel;
		player_velocity = 0;
		is_PLAYER_jumping = false;
	}
}


//player movement
void static PLAYER_BALL_FUNCTION() {
	PLAY_PLAYER_IDLE();
	if (IsKeyDown(KEY_A)) { PLAYER_POS.x -= 500 * GetFrameTime(); player_dir = -1; }
	if (IsKeyDown(KEY_D)) { PLAYER_POS.x += 500 * GetFrameTime(); player_dir = 1;  }
	JUMP();
	PLAYER_POS.x = Clamp(PLAYER_POS.x, -1500, 1500);

	
		DrawTexturePro(
			PLAYER[currentFrame_player],
			{ 0 , 0 , (float)PLAYER[currentFrame_player].width * player_dir , (float)PLAYER[currentFrame_player].height },
			{ (float)(PLAYER_POS.x - (float)PLAYER[currentFrame_player].width * 1.5 / 2),
			  (float)(PLAYER_POS.y - (float)PLAYER[currentFrame_player].height * 1.5 / 2) + 240 ,
			(float)PLAYER[currentFrame_player].width , (float)PLAYER[currentFrame_player].height },
			{ 0 , 0 }, 0, WHITE
		);
}


//players projectile
void static PROJECTILES_FUNCTION() {
	fire_rate -= GetFrameTime();

	//for left fire button
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && fire_rate <= 0 && Player_Bullets > 0) {
		player_dir = -1;
		for (int i = 0; i < MAX_BULLET; i++) {
			if (!NUM_BULLET[i].Is_BULLET_active) {
				NUM_BULLET[i].POS = PLAYER_POS;
				fire_rate = 0.20f;

				NUM_BULLET[i].Is_BULLET_active = true;
                NUM_BULLET[i].VEL = { bullet_speed * player_dir , 0 };
				Player_Bullets -= 1;
				
				PlaySound(GUN_SHOT);
				break;
			}
		}
	}

	//for right fire button
	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && fire_rate <= 0 && Player_Bullets > 0) {
		player_dir = 1;
		for (int i = 0; i < MAX_BULLET; i++) {
			if (!NUM_BULLET[i].Is_BULLET_active) {
				NUM_BULLET[i].POS = PLAYER_POS;
				fire_rate = 0.20f;
				
				NUM_BULLET[i].Is_BULLET_active = true;
				NUM_BULLET[i].VEL = { bullet_speed * player_dir , 0 };
				Player_Bullets -= 1;

				PlaySound(GUN_SHOT);
				break;
			}
		}
	}

	for (int i = 0; i < MAX_BULLET; i++) {
		if (NUM_BULLET[i].Is_BULLET_active) {

			NUM_BULLET[i].POS += NUM_BULLET[i].VEL;
			DrawTextureEx(
				BULLET,
				{ NUM_BULLET[i].POS.x - BULLET.width * 3 / 2,
				  (NUM_BULLET[i].POS.y - BULLET.height * 3 / 2) + 240 },
				0, 3.0f, WHITE
			);
		}
		if (NUM_BULLET[i].POS.x > 1900 || NUM_BULLET[i].POS.x < -1900) {
			NUM_BULLET[i].Is_BULLET_active = false;
		}
	}

	if (Player_Bullets <= 0) {
		for (int i = 0; i < MAX_BULLET; i++) {
			NUM_BULLET[i].Is_BULLET_active = false;
		}
	}
}


//making enemies
void static SPWAN_ENEMIES_FUNCTION() {
	static int random;
	random = GetRandomValue(0, 1);
	
	for (int i = 0; i < MAX_ENEMY; i++) {
		
		Vector2 pos;
		Vector2 vel;

		if (!NUM_ENEMY[i].Is_ENEMY_active) {
			if (random == 0) vel = { 4 , 4 };
			if (random == 1) vel = { -4 , 4 };
			
			switch (random) {
			case 0 :            //left side
				pos = { -2000 , 630};
				break;

			case 1:             //right side
				pos = { 3000 , 630 };
				break;
			}

			NUM_ENEMY[i].POS = pos;
			NUM_ENEMY[i].VEL.x = vel.x;
			NUM_ENEMY[i].hp = NUM_ENEMY[i].Max_hp = 100;
			NUM_ENEMY[i].Is_ENEMY_active = true;
			break;
		}
	}
}


//spwaning enemy
void static UPDATE_ENEMY_FUNCTION() {
	ENEMY_ANIMATION_PLAY();

	for (int i = 0; i < MAX_ENEMY; i++) {
		if (NUM_ENEMY[i].Is_ENEMY_active) {
			Vector2 dir = Vector2Normalize(Vector2Subtract(PLAYER_POS, NUM_ENEMY[i].POS));

			float speed = fabs(NUM_ENEMY[i].VEL.x);
			Vector2 move = { dir.x * speed, 0};
			NUM_ENEMY[i].POS = Vector2Add(NUM_ENEMY[i].POS , move);
			
			//enemy facing right
			if (dir.x > 0) {
				DrawTexturePro(
					ENEMY_RUN[currentFrame],
					{ 0 , 0 , (float)ENEMY_RUN[currentFrame].width , (float)ENEMY_RUN[currentFrame].height },
					{ NUM_ENEMY[i].POS.x - (float)ENEMY_RUN[currentFrame].width * 1 / 2,
					  (NUM_ENEMY[i].POS.y - (float)ENEMY_RUN[currentFrame].height * 1 / 2) + 240 ,
					(float)ENEMY_RUN[currentFrame].width , (float)ENEMY_RUN[currentFrame].height },
					{ 0 , 0 }, 0, WHITE
				);
			} 
			//enemy facing left
			else {
				DrawTexturePro(
					ENEMY_RUN[currentFrame],
					{ 0 , 0 , (float)-ENEMY_RUN[currentFrame].width , (float)ENEMY_RUN[currentFrame].height },
					{ NUM_ENEMY[i].POS.x - (float)ENEMY_RUN[currentFrame].width * 1 / 2,
					  (NUM_ENEMY[i].POS.y - (float)ENEMY_RUN[currentFrame].height * 1 / 2) + 240 ,
					(float)ENEMY_RUN[currentFrame].width , (float)ENEMY_RUN[currentFrame].height } ,
					{0 , 0 } , 0 , WHITE
				);
			}
		}
	}
}


//checking if bullet hit the enemy
void static CHECK_HIT_ENEMY_FUNCTION() {
	ENEMY_ANIMATION_PLAY();
	float Bullet_radius = BULLET.width * 1.5;
	float Enemy_radius = ENEMY_RUN[0].width * 0.5;

	for (int i = 0; i < MAX_BULLET; i++) {
		if (!NUM_BULLET[i].Is_BULLET_active) continue;

		for (int j = 0; j < MAX_ENEMY; j++) {
			if (!NUM_ENEMY[j].Is_ENEMY_active) continue;

			float DIST = Vector2Distance(NUM_BULLET[i].POS, NUM_ENEMY[j].POS);

			if (DIST < Bullet_radius + Enemy_radius) {
				NUM_ENEMY[j].hp -= 50;
				PlaySound(ZOMBIE);
				NUM_BULLET[i].Is_BULLET_active = false;       //bullet disapears

				if (NUM_ENEMY[j].hp <= 0) {
					NUM_ENEMY[j].Is_ENEMY_active = false;     //enemy disapears
					score += 1;
				}
				break;
			}
		}
	}
}


//speed of enemies
void static SPEED(int speed_rate) {
	static int Frame = 0;
	static int Last_Frame = 0;

	if (Last_Frame != speed_rate) {
		Frame = 0;
		Last_Frame = speed_rate;
	}

	Frame++;
	if (Frame >= speed_rate) {
		Frame = 0;
		SPWAN_ENEMIES_FUNCTION();
	}
}


//determies speed of enemies based on the wave
void static TIME_SPEED_FUNCTION() {

	if (CurrentState == WAVE1 || (CurrentState == GAME && score < 10)) {
		SPEED(30);
	}
	else if (CurrentState == WAVE2 || (CurrentState == GAME && score >= 10)) {
		SPEED(25);
	}
	else if (CurrentState == WAVE3 || (CurrentState == GAME && score >= 20)) {
		SPEED(20);
	}
	else if (CurrentState == WAVE4 || (CurrentState == GAME && score >= 30)) {
		SPEED(20);
	}
}


//check if player got hit
void PLAYER_HIT_FUNCTION() {
	bool hit = false;

	for (int i = 0; i < MAX_ENEMY; i++) {
		if (NUM_ENEMY[i].Is_ENEMY_active && CheckCollisionPointCircle(NUM_ENEMY[i].POS, PLAYER_POS , 50)) {
			Health -= 2;
			hit = true;
			NUM_ENEMY[i].Is_ENEMY_active = false;
		}
	}
}


//enemy health bar
void HEALTH_BAR_ENEMY() {
	CHECK_HIT_ENEMY_FUNCTION();
	for (int i = 0; i < MAX_ENEMY; i++) {
		if (!NUM_ENEMY[i].Is_ENEMY_active) continue;         //checks if enemy is alive

		if (NUM_ENEMY[i].Max_hp <= 0) {                      //protection from dividing by zero
			NUM_ENEMY[i].Max_hp = 1;
		}
		if (NUM_ENEMY[i].hp < 0) NUM_ENEMY[i].hp = 0;        //clamping hp 
		if (NUM_ENEMY[i].hp > NUM_ENEMY[i].Max_hp) {         //Also clamping hp
			NUM_ENEMY[i].hp = NUM_ENEMY[i].Max_hp;
		}

		Rectangle Health_bar = { NUM_ENEMY[i].POS.x , NUM_ENEMY[i].POS.y , 100 , 20};

		float BarX = Health_bar.x;
		float BarY = Health_bar.y + 150;
		float BarWidth = Health_bar.width;
		float BarHeight = 8;

		DrawRectangle(BarX, BarY, BarWidth, BarHeight, BLANK);
		float ratio = (float)NUM_ENEMY[i].hp / (float)NUM_ENEMY[i].Max_hp;

		DrawRectangle(BarX, BarY, BarWidth * ratio, BarHeight, RED);
	}
}


//game start 
void START_OF_GAME() {
	camara.target = PLAYER_POS;
	BeginMode2D(camara);

	BACKGROUND_SCENARIO();

	PLAYER_BALL_FUNCTION();
	PROJECTILES_FUNCTION();
	TIME_SPEED_FUNCTION();
	PLAYER_HIT_FUNCTION();
	UPDATE_ENEMY_FUNCTION();
	HEALTH_BAR_ENEMY();

	EndMode2D();

	game_time += GetFrameTime();
	HEALTH();
	DrawText(TextFormat("TIME : %.2f", game_time), 10, 50, 30, BLACK);
	DrawText(TextFormat("SCORE : %d", score), 10, 10, 30, BLACK);
	DrawText(TextFormat("BULLETS : %d", Player_Bullets), 750, 50, 30, BLACK);

	
	SHOPING();

	if (IsKeyPressed(KEY_ESCAPE)) {
		CurrentState = PAUSE;
	}
}


//starts playing wave animation[text]
void WAVE_TEXT(int wave_number) {
	
	static float Start_time = 0;
	static int Previous_wave = 0;

	if (Previous_wave != wave_number) {
		Previous_wave = wave_number;
		Start_time = 0;
	}

	Start_time += GetFrameTime();

	if (Start_time < 3) {
		DrawText(TextFormat("WAVE %d IS STARTING" , wave_number), 90, 100, 50, BLACK);
	}	
}


//shoping state 
void SHOPING() {

	if (CurrentState == WAVE2 && !INDEX[0].Next_Stage) {
		CurrentState = Q1;
		INDEX[0].Next_Stage = true;
	}

	if (CurrentState == WAVE3 && !INDEX[1].Next_Stage) {
		CurrentState = Q8;
		INDEX[1].Next_Stage = true;
	}

	if (CurrentState == WAVE4 && !INDEX[2].Next_Stage) {
		CurrentState = Q15;
		INDEX[2].Next_Stage = true;
	}
	
}


//next state 
void NEXT(State NextState) {
	Rectangle next = { 380 , 700 , 200 , 70 };
	Color color_next = CheckCollisionPointRec(MOUSEPOSITION, next) ? GRAY : GREEN;
	DrawRectangleRec(next, color_next);
	DrawText("NEXT", next.x + 30, next.y + 10, 50, BLACK);

	if (CheckCollisionPointRec(MOUSEPOSITION, next) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		CurrentState = NextState;
		CO1 = GRAY;
		CO2 = GRAY;
		CO3 = GRAY;
		CO4 = GRAY;
		timer = 0;
		selected_option = 0;
	}
}


//making options
void OPTIONS() {
	//checking which option is selected

	if (CheckCollisionPointRec(MOUSEPOSITION, O1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		selected_option = (selected_option == 1) ? 0 : 1;
	}

	if (CheckCollisionPointRec(MOUSEPOSITION, O2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		selected_option = (selected_option == 2) ? 0 : 2;
	}

	if (CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		selected_option = (selected_option == 3) ? 0 : 3;
	}

	if (CheckCollisionPointRec(MOUSEPOSITION, O4) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		selected_option = (selected_option == 4) ? 0 : 4;
	}

	//assignung colors
	CO1 = (selected_option == 1) ? GREEN : GRAY;
	CO2 = (selected_option == 2) ? GREEN : GRAY;
	CO3 = (selected_option == 3) ? GREEN : GRAY;
	CO4 = (selected_option == 4) ? GREEN : GRAY;
}


//rectangles of the option
void OPTION_BUTTON() {
	OPTIONS();
	DrawRectangleRec(O1, CO1);
	DrawRectangleRec(O2, CO2);
	DrawRectangleRec(O3, CO3);
	DrawRectangleRec(O4, CO4);
}

//========================WAVE 1================================//

//bullet
void QUESTION1() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[0], 100, 100, 40, BLACK);
	DrawText(Options[0], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[1], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[2], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[3], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[0].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[0].is_OPTION_selected = true;
	}

	if (!INDEX[0].one_option && CheckCollisionPointRec(MOUSEPOSITION, O1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets += 70;
		
		INDEX[0].one_option = true;
	}

	if (INDEX[0].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O3) || CheckCollisionPointRec(MOUSEPOSITION, O4)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets -= 70;
		
		INDEX[0].one_option = false;
	}

	if (INDEX[0].is_OPTION_selected) {
		NEXT(Q2);
	}
}


//health
void QUESTION2() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[1], 100, 100, 40, BLACK);
	DrawText(Options[4], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[5], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[6], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[7], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[1].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[1].is_OPTION_selected = true;
	}

	if (!INDEX[1].one_option && CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health += 10;
		Health = Clamp(Health, 0, 100);
		INDEX[1].one_option = true;
	}

	if (INDEX[1].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O1) || CheckCollisionPointRec(MOUSEPOSITION, O4)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health -= 10;
		INDEX[1].one_option = false;
	}

	if (INDEX[1].is_OPTION_selected) {
		NEXT(Q3);
	}
}


//bullet
void QUESTION3() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[2], 100, 100, 50, BLACK);
	DrawText(Options[8], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[9], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[10], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[11], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[2].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[2].is_OPTION_selected = true;
	}

	if (!INDEX[2].one_option && CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets += 70;
		
		INDEX[2].one_option = true;
	}

	if (INDEX[2].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets -= 50;
		
		INDEX[2].one_option = false;
	}

	if (INDEX[2].is_OPTION_selected) {
		NEXT(Q4);
	}
}


//health
void QUESTION4() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[3], 100, 100, 30, BLACK);
	DrawText(Options[12], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[13], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[14], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[15], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[3].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[3].is_OPTION_selected = true;
	}

	if (!INDEX[3].one_option && CheckCollisionPointRec(MOUSEPOSITION, O2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health += 10;
		Health = Clamp(Health, 0, 100);
		INDEX[3].one_option = true;
	}

	if (INDEX[3].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O3) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health -= 10;
		INDEX[3].one_option = false;
	}

	if (INDEX[3].is_OPTION_selected) {
		NEXT(Q5);
	}
}


//bullet speed
void QUESTION5() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[4], 100, 100, 30, BLACK);
	DrawText(Options[16], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[17], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[18], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[19], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[4].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[4].is_OPTION_selected = true;
	}

	if (!INDEX[4].one_option && CheckCollisionPointRec(MOUSEPOSITION, O4) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		bullet_speed += 5.0f;
		INDEX[4].one_option = true;
	}

	if (INDEX[4].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O3) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		bullet_speed -= 5.0f;
		INDEX[4].one_option = false;
	}

	if (INDEX[4].is_OPTION_selected) {
		NEXT(Q6);
	}
}


//bullet speed
void QUESTION6() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[5], 100, 100, 30, BLACK);
	DrawText(Options[20], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[21], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[22], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[23], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[5].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[5].is_OPTION_selected = true;
	}

	if (!INDEX[5].one_option && CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		bullet_speed += 5.0f;
		INDEX[5].one_option = true;
	}

	if (INDEX[5].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		bullet_speed -= 5.0f;
		INDEX[5].one_option = false;
	}

	if (INDEX[5].is_OPTION_selected) {
		NEXT(Q7);
	}
}


//bullet + Bullet speed
void QUESTION7() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[6], 100, 100, 30, BLACK);
	DrawText(Options[24], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[25], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[26], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[27], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[6].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[6].is_OPTION_selected = true;
	}

	if (!INDEX[6].one_option && CheckCollisionPointRec(MOUSEPOSITION, O2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets += 70;
		bullet_speed += 3.0f;
		INDEX[6].one_option = true;
	}

	if (INDEX[6].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O3) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets -= 40;
		bullet_speed -= 3.0f;
		INDEX[6].one_option = false;
	}

	if (INDEX[6].is_OPTION_selected) {
		MAKE_BUTTON({ 380 , 700 , 350 , 70 }, GRAY, GREEN, "CONTINUE", 30, 10, 50, WAVE1, false);
	}
}

//==============================================================//

//=========================WAVE 2===============================//

//bullet
void QUESTION8() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[7], 100, 100, 30, BLACK);
	DrawText(Options[28], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[29], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[30], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[31], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[7].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[7].is_OPTION_selected = true;
	}

	if (!INDEX[7].one_option && CheckCollisionPointRec(MOUSEPOSITION, O2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets += 70;
		INDEX[7].one_option = true;
	}

	if (INDEX[7].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O3) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets -= 40;
		INDEX[7].one_option = false;
	}

	if (INDEX[7].is_OPTION_selected) {
		NEXT(Q9);
	}
}


//health
void QUESTION9() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[8], 100, 100, 30, BLACK);
	DrawText(Options[32], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[33], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[34], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[35], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[8].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[8].is_OPTION_selected = true;
	}

	if (!INDEX[8].one_option && CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health += 20;
		Health = Clamp(Health, 0, 100);
		INDEX[8].one_option = true;
	}

	if (INDEX[8].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health -= 20;
		INDEX[8].one_option = false;
	}

	if (INDEX[8].is_OPTION_selected) {
		NEXT(Q10);
	}
}


//bullet speed
void QUESTION10() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[9], 100, 100, 30, BLACK);
	DrawText(Options[36], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[37], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[38], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[39], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[9].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[9].is_OPTION_selected = true;
	}

	if (!INDEX[9].one_option && CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		bullet_speed += 6.0f;
		INDEX[9].one_option = true;
	}

	if (INDEX[9].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		bullet_speed -= 3.0f;
		INDEX[9].one_option = false;
	}

	if (INDEX[9].is_OPTION_selected) {
		NEXT(Q11);
	}
}


//bullet
void QUESTION11() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[10], 100, 100, 30, BLACK);
	DrawText(Options[40], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[41], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[42], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[43], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[10].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[10].is_OPTION_selected = true;
	}

	if (!INDEX[10].one_option && CheckCollisionPointRec(MOUSEPOSITION, O4) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets += 70;
		
		INDEX[10].one_option = true;
	}

	if (INDEX[10].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O1) || CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets -= 30;
		
		INDEX[10].one_option = false;
	}

	if (INDEX[10].is_OPTION_selected) {
		NEXT(Q12);
	}
}


//Health
void QUESTION12() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[11], 100, 100, 30, BLACK);
	DrawText(Options[44], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[45], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[46], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[47], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[11].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[11].is_OPTION_selected = true;
	}

	if (!INDEX[11].one_option && CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health += 20;
		INDEX[11].one_option = true;
	}

	if (INDEX[11].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health -= 20;
		INDEX[11].one_option = false;
	}

	if (INDEX[11].is_OPTION_selected) {
		NEXT(Q13);
	}
}


//bullet + speed
void QUESTION13() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[12], 100, 100, 30, BLACK);
	DrawText(Options[48], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[49], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[50], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[51], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[12].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[12].is_OPTION_selected = true;
	}

	if (!INDEX[12].one_option && CheckCollisionPointRec(MOUSEPOSITION, O1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets += 70;
		bullet_speed += 5.0f;
		INDEX[12].one_option = true;
	}

	if (INDEX[12].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O3)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets -= 70;
		bullet_speed -= 5.0f;
		INDEX[12].one_option = false;
	}

	if (INDEX[12].is_OPTION_selected) {
		NEXT(Q14);
	}
}


//Health
void QUESTION14() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[13], 100, 100, 30, BLACK);
	DrawText(Options[52], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[53], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[54], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[55], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[13].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[13].is_OPTION_selected = true;
	}

	if (!INDEX[13].one_option && CheckCollisionPointRec(MOUSEPOSITION, O1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health += 20;
		INDEX[13].one_option = true;
	}

	if (INDEX[13].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O3)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health -= 20;
		INDEX[13].one_option = false;
	}

	if (INDEX[13].is_OPTION_selected) {
		MAKE_BUTTON({ 380 , 700 , 350 , 70 }, GRAY, GREEN, "CONTINUE", 30, 10, 50, WAVE1, false);
	}
}

//==============================================================//

//====================WAVE 3====================================//


//bullet
void QUESTION15() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[14], 100, 100, 30, BLACK);
	DrawText(Options[56], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[57], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[58], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[59], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[14].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[14].is_OPTION_selected = true;
	}

	if (!INDEX[14].one_option && CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets += 70;
		
		INDEX[14].one_option = true;
	}

	if (INDEX[14].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets -= 40;
		
		INDEX[14].one_option = false;
	}

	if (INDEX[14].is_OPTION_selected) {
		NEXT(Q16);
	}
}


//bullet speed
void QUESTION16() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[15], 100, 100, 30, BLACK);
	DrawText(Options[60], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[61], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[62], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[63], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[15].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[15].is_OPTION_selected = true;
	}

	if (!INDEX[15].one_option && CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
	
		bullet_speed += 6.0f;
		INDEX[15].one_option = true;
	}

	if (INDEX[15].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		bullet_speed -= 3.0f;
		INDEX[15].one_option = false;
	}

	if (INDEX[15].is_OPTION_selected) {
		NEXT(Q17);
	}
}


//Health
void QUESTION17() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[16], 100, 100, 30, BLACK);
	DrawText(Options[64], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[65], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[66], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[67], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[16].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[16].is_OPTION_selected = true;
	}

	if (!INDEX[16].one_option && CheckCollisionPointRec(MOUSEPOSITION, O1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health += 20;
		INDEX[16].one_option = true;
	}

	if (INDEX[16].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O3)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health -= 20;
		INDEX[16].one_option = false;
	}

	if (INDEX[16].is_OPTION_selected) {
		NEXT(Q18);
	}
}


//bullet + speed
void QUESTION18() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[17], 100, 100, 30, BLACK);
	DrawText(Options[68], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[69], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[70], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[71], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[17].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[17].is_OPTION_selected = true;
	}

	if (!INDEX[17].one_option && CheckCollisionPointRec(MOUSEPOSITION, O3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets += 70;
		bullet_speed += 5.0f;
		INDEX[17].one_option = true;
	}

	if (INDEX[17].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets -= 40;
		bullet_speed -= 5.0f;
		INDEX[17].one_option = false;
	}

	if (INDEX[17].is_OPTION_selected) {
		NEXT(Q19);
	}
}


//Health
void QUESTION19() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[18], 100, 100, 30, BLACK);
	DrawText(Options[72], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[73], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[74], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[75], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[18].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[18].is_OPTION_selected = true;
	}

	if (!INDEX[18].one_option && CheckCollisionPointRec(MOUSEPOSITION, O2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health += 20;
		INDEX[18].one_option = true;
	}

	if (INDEX[18].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O3) || CheckCollisionPointRec(MOUSEPOSITION, O4) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Health -= 20;
		INDEX[18].one_option = false;
	}

	if (INDEX[18].is_OPTION_selected) {
		NEXT(Q20);
	}
}


//bullet + speed
void QUESTION20() {
	OPTION_BUTTON();
	timer += GetFrameTime();
	DrawText(TextFormat("TIMER : %.2f", timer), 10, 10, 30, BLACK);

	DrawText(Question[19], 100, 100, 30, BLACK);
	DrawText(Options[76], O1.x + 45, O1.y + 15, 30, BLACK);
	DrawText(Options[77], O2.x + 45, O2.y + 15, 30, BLACK);
	DrawText(Options[78], O3.x + 45, O3.y + 15, 30, BLACK);
	DrawText(Options[79], O4.x + 45, O4.y + 15, 30, BLACK);

	if (!INDEX[19].is_OPTION_selected && (CheckCollisionPointRec(MOUSEPOSITION, O1) ||
		CheckCollisionPointRec(MOUSEPOSITION, O2) ||
		CheckCollisionPointRec(MOUSEPOSITION, O3) ||
		CheckCollisionPointRec(MOUSEPOSITION, O4)) &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		INDEX[19].is_OPTION_selected = true;
	}

	if (!INDEX[19].one_option && CheckCollisionPointRec(MOUSEPOSITION, O4) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets += 70;
		bullet_speed += 5.0f;
		INDEX[19].one_option = true;
	}

	if (INDEX[19].one_option && (CheckCollisionPointRec(MOUSEPOSITION, O2) || CheckCollisionPointRec(MOUSEPOSITION, O3) || CheckCollisionPointRec(MOUSEPOSITION, O1)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Player_Bullets -= 40;
		bullet_speed -= 5.0f;
		INDEX[19].one_option = false;
	}

	if (INDEX[9].is_OPTION_selected) {
		MAKE_BUTTON({ 380 , 700 , 350 , 70 }, GRAY, GREEN, "CONTINUE", 30, 10, 50, WAVE1, false);
	}
}

//=============================================================//


//game over and back to menu button
void static GAME_OVER_FUNCTION() {
	DrawText("GAME OVER!!", 250, 200, 100, RED);
	DrawText(TextFormat("SCORE : %d", score), 350, 350, 50, RED);
	PLAY_AGAIN();
	BACK_FUNCTION();
	BORDER_FUNCTION();
}


//end of game
void END_OF_GAME() {
	DrawText("YOU SURVIED", 340, 100, 60, GOLD);
	DrawText(TextFormat("SCORE : %d", score), 350, 350, 50, RED);
	BACK_FUNCTION();
	PLAY_AGAIN();
}
