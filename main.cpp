
/* TO DO
    - Fix I-Tetromino wall kick***
        -and various other kick bugs
    - Implement Hard Drop
    - Title screen
        - Options choosing starting level and music
    - End game screen
        - Offer continue, options or quit
*/

//hio

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <cmath>

//Global Constants
static int SCREEN_WIDTH = 1280;
static int SCREEN_HEIGHT = 800;

//Board constants
static int ROWS = 22;
static int COLUMNS = 10;
static int SCORE_WIDTH = 5;
static int SCORE_HEIGHT = 2;
static int BRICK_LENGTH = 32;
static int BACKGROUND = 7;
static int BOARD_LEFT = ( SCREEN_WIDTH - ( COLUMNS * BRICK_LENGTH ) ) / 2;
static int BOARD_TOP = ( SCREEN_HEIGHT - ( ROWS * BRICK_LENGTH ) ) / 2;
static int SCORE_LEFT = ( SCREEN_WIDTH - ( SCORE_WIDTH * BRICK_LENGTH ) ) / 4;
static int SCORE_TOP = ( SCREEN_HEIGHT - ( SCORE_HEIGHT * BRICK_LENGTH ) ) / 4;
static int SCORE_START = 4;

//Enumeration for Tetromino types
enum TetrominoType
{
    TETRO_I = 0,
    TETRO_T = 1,
    TETRO_L = 2,
    TETRO_J = 3,
    TETRO_S = 4,
    TETRO_Z = 5,
    TETRO_O = 6,
    TETRO_TOTAL = 7
};

//Enumeration for rotation states
enum TetrominoRotation
{
    ROTATION_1 = 0,
    ROTATION_2 = 1,
    ROTATION_3 = 2,
    ROTATION_4 = 3,
    ROTATION_SPAWN = 4
};

//Vector containing Tetromino shapes
std::vector< std::vector< std::vector< std::vector< int > > > > shapeArrays {
    //Rotation States
    std::vector< std::vector< std::vector< int > > > {
        //I-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, TETRO_I, BACKGROUND },
            std::vector< int > { BACKGROUND, BACKGROUND, TETRO_I, BACKGROUND },
            std::vector< int > { BACKGROUND, BACKGROUND, TETRO_I, BACKGROUND },
            std::vector< int > { BACKGROUND, BACKGROUND, TETRO_I, BACKGROUND } },
        //T-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_T, BACKGROUND },
            std::vector< int > { TETRO_T, TETRO_T, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_T, BACKGROUND } },
        //L-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { TETRO_L, TETRO_L, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_L, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_L, BACKGROUND } },
        //J-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_J, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_J, BACKGROUND },
            std::vector< int > { TETRO_J, TETRO_J, BACKGROUND } },
        //S-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_S, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_S, TETRO_S },
            std::vector< int > { BACKGROUND, BACKGROUND, TETRO_S } },
        //Z-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, TETRO_Z },
            std::vector< int > { BACKGROUND, TETRO_Z, TETRO_Z },
            std::vector< int > { BACKGROUND, TETRO_Z, BACKGROUND } },
    },
    
    std::vector< std::vector< std::vector< int > > > {
        //I-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_I, TETRO_I, TETRO_I, TETRO_I },
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND } },
        //T-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_T, BACKGROUND },
            std::vector< int > { TETRO_T, TETRO_T, TETRO_T } },
        //L-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, BACKGROUND, TETRO_L },
            std::vector< int > { TETRO_L, TETRO_L, TETRO_L } },
        //J-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_J, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_J, TETRO_J, TETRO_J } },
        //S-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_S, TETRO_S },
            std::vector< int > { TETRO_S, TETRO_S, BACKGROUND } },
        //Z-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_Z, TETRO_Z, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_Z, TETRO_Z } },
    },
    
    std::vector< std::vector< std::vector< int > > > {
        //I-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_I, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_I, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_I, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_I, BACKGROUND, BACKGROUND } },
        //T-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_T, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_T, TETRO_T },
            std::vector< int > { BACKGROUND, TETRO_T, BACKGROUND } },
        //L-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_L, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_L, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_L, TETRO_L } },
        //J-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_J, TETRO_J },
            std::vector< int > { BACKGROUND, TETRO_J, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_J, BACKGROUND } },
        //S-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { TETRO_S, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_S, TETRO_S, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_S, BACKGROUND } },
        //Z-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_Z, BACKGROUND },
            std::vector< int > { TETRO_Z, TETRO_Z, BACKGROUND },
            std::vector< int > { TETRO_Z, BACKGROUND, BACKGROUND } },
    },
    
    std::vector< std::vector< std::vector< int > > > {
        //I-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_I, TETRO_I, TETRO_I, TETRO_I },
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND } },
        //T-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_T, TETRO_T, TETRO_T },
            std::vector< int > { BACKGROUND, TETRO_T, BACKGROUND } },
        //L-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_L, TETRO_L, TETRO_L },
            std::vector< int > { TETRO_L, BACKGROUND, BACKGROUND } },
        //J-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_J, TETRO_J, TETRO_J },
            std::vector< int > { BACKGROUND, BACKGROUND, TETRO_J } },
        //S-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_S, TETRO_S },
            std::vector< int > { TETRO_S, TETRO_S, BACKGROUND } },
        //Z-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND },
            std::vector< int > { TETRO_Z, TETRO_Z, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_Z, TETRO_Z } },
    },
    
    //Spawn arrays
    std::vector< std::vector< std::vector< int > > > {
        //I-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { TETRO_I, TETRO_I, TETRO_I, TETRO_I },
            std::vector< int > { BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND } },
        //T-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { TETRO_T, TETRO_T, TETRO_T, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_T, BACKGROUND, BACKGROUND } },
        //L-Tetro
        std::vector< std::vector< int > >{
            std::vector< int > { TETRO_L, TETRO_L, TETRO_L, BACKGROUND },
            std::vector< int > { TETRO_L, BACKGROUND, BACKGROUND, BACKGROUND } },
        //J-Tetro
        std::vector< std::vector< int > >{
            std::vector< int > { TETRO_J, TETRO_J, TETRO_J, BACKGROUND },
            std::vector< int > { BACKGROUND, BACKGROUND, TETRO_J, BACKGROUND } },
        //S-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_S, TETRO_S, BACKGROUND },
            std::vector< int > { TETRO_S, TETRO_S, BACKGROUND, BACKGROUND } },
        //Z-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { TETRO_Z, TETRO_Z, BACKGROUND, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_Z, TETRO_Z, BACKGROUND } },
        //O-Tetro
        std::vector< std::vector< int > > {
            std::vector< int > { BACKGROUND, TETRO_O, TETRO_O, BACKGROUND },
            std::vector< int > { BACKGROUND, TETRO_O, TETRO_O, BACKGROUND } }
    }
};

//Texture object and related functions
class Texture
{
public:
    //Initializes variables
    Texture();
    
    //Deallocates memory
    ~Texture();
    
    //Loads image at specified path
    bool loadFromFolder( std::string path );
    
    //Deallocates texture
    void free();
    
    //Render texture at given point
    void render( int x, int y, SDL_Rect* clip = NULL );
    
    //Get image dimensions
    int getWidth();
    int getHeight();
    
private:
    //The actual texture
    SDL_Texture* texture;
    
    //Image dimensions
    int width;
    int height;
};

class Timer
{
public:
    //Initialize variables
    Timer();
    
    //The various timer actions
    void start();
    void stop();
    void pause();
    void unpause();
    
    //Get the timer's time
    Uint32 getTicks();
    
    //Check the status of timer
    bool isStarted();
    bool isPaused();
    
private:
    //The clock time when the timer is started
    Uint32 startTicks;
    
    //The ticks stored when the timer is paused
    Uint32 pauseTicks;
    
    //The timer status
    bool started;
    bool paused;
};

class Tetromino
{
public:
    //Initializes variables
    Tetromino();
    
    //Handle user input for the tetromino
    void handleEvent( SDL_Event &e );
    
    //Handle logic
    void logic();
    
    //Render tetromino to the screen
    void render();
    
private:
    //Game ended flag
    bool gameOver;
    
    //Has a held Tetromino
    bool hasHeld;
    
    //If current Tetromino was held
    bool wasHeld;
    
    //Random number generator
    std::mt19937* RNG;
    
    //The current level
    int level;
    
    //The current and max points needed to level up
    int currentPoints;
    int pointsNeeded;
    
    //The current speed(time to fall one space in milliseconds)
    int speed;
    
    //The current score
    int score;
    
    //The lock delay
    int delay;
    
    //The current positions of the top left of the Tetromino's array
    int tetroTop, tetroLeft;
    
    //The game board
    std::vector< std::vector< int > > board;
    
    //Bag holding next pieces
    std::vector< int > nextBag;
    
    //Move timer
    Timer moveTimer;
    
    //Lock delay timer
    Timer lockDelayTimer;
    
    //Lock delay timer resets
    int lockDelayResets;
    int maxLockDelayResets;
    
    //Tetromino type and rotation state
    int type;
    int heldType;
    int rotation;
    
    //Create and initialize values for new tetromino
    void spawn( bool held );
    
    //Make the tetromino fall
    void fall();
    
    //Move the current Tetromino based on user input
    void move( int offset );
    
    //Check for any collisions
    bool checkCollisions( int potentialTetroLeft, int potentialTetroTop, int potentialRotation );
    
    //Change current orientation
    void rotate( bool counterclock );
    
    //Attempt to wall and floor kick
    bool tryKick( bool counterclock, int potentialRotation );
    
    //Try to perform a wallkick
    bool wallKick( bool counterclock, int potentialRotation );
    
    //Check for and handle completed line
    void completeLines();
    
    //Increase the level
    void levelUp();
    
    //Check if the game has ended
    void checkOver();
    
    //Hold current Tetromino
    void hold();
    
    //Reset the game
    void reset();
    
    //Get current board state
    std::vector< std::vector< int > > getBoard();
    
    //Fill the bag
    void fillBag();
    
    //Render current tetromino
    void renderTetromino();
    
    //Render game board
    void renderBoard();
    
    //Render score board
    void renderScore();
    
    //Render the next Tetrominos
    void renderNext();
    
    //Render the held Tetromino
    void renderHeld();
};

//Start SDL and create window
bool init();

//Load media
bool loadMedia();

//Free media and close SDL
void close();

//Window to be rendered to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Textures to render
Texture gSpriteSheetTeture;

//Music to play
Mix_Music *gMusic = NULL;

//////////  Texture Methods  //////////

Texture::Texture()
{
    //Initialization
    texture = NULL;
    width = 0;
    height = 0;
}

Texture::~Texture()
{
    //Deallocate
    free();
}

bool Texture::loadFromFolder( std::string path )
{
    //Free previous texture
    free();
    
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Load image at the specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            width = loadedSurface->w;
            height = loadedSurface->h;
        }
        
        //Get rid of temp surface
        SDL_FreeSurface( loadedSurface );
    }
    
    //Return success
    texture = newTexture;
    return texture != NULL;
}

void Texture::free()
{
    //Free texture if it exists
    if( texture != NULL )
    {
        SDL_DestroyTexture( texture );
        texture = NULL;
        width = 0;
        height = 0;
    }
}

void Texture::render( int x, int y, SDL_Rect* clip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, width, height };
    
    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    //Render to screen
    SDL_RenderCopy( gRenderer, texture, clip, &renderQuad );
}

int Texture::getWidth()
{
    return width;
}

int Texture::getHeight()
{
    return height;
}

//////////  Timer Methods  //////////

Timer::Timer()
{
    //Initialize variables
    startTicks = 0;
    pauseTicks = 0;
    
    started = false;
    paused = false;
}

void Timer::start()
{
    //Start the timer
    started = true;
    
    //Unpause the timer
    paused = false;
    
    //Get the current clock time
    startTicks = SDL_GetTicks();
    pauseTicks = 0;
}

void Timer::stop()
{
    //Stop the timer
    started = false;
    
    //Unpause the timer
    paused = false;
    
    //Clear variables
    startTicks = 0;
    pauseTicks = 0;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( started && !paused )
    {
        //Pause the timer
        paused = true;
        
        //Calculater the pause ticks
        pauseTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused )
    {
        //Unpause the timer
        paused = false;
        
        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pauseTicks;
        
        //Reset the pause ticks
        pauseTicks = 0;
    }
}

Uint32 Timer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;
    
    //If the timer is running
    if( started )
    {
        if( paused )
        {
            time = pauseTicks;
        }
        else
        {
            time = SDL_GetTicks() - startTicks;
        }
    }
    
    return time;
}

bool Timer::isStarted()
{
    //Timer is running and paused or unpaused
    return started;
}

bool Timer::isPaused()
{
    //Timer is running and paused
    return started && paused;
}

////////// Tetromino Methods //////////

Tetromino::Tetromino()
{
    //Game over flag
    gameOver = false;
    
    //Seeding the random number generator
    unsigned int seed = static_cast< int >( std::chrono::system_clock::now().time_since_epoch().count() );
    RNG = new std::mt19937( seed );
    
    //Initialize variables
    spawn( false );
    
    //Initialize the board
    std::vector< std::vector< int > >initBoard ( ROWS, std::vector< int >( COLUMNS, BACKGROUND ) );
    board = initBoard;
    
    //Initialize game variables
    level = 1;
    currentPoints = 0;
    pointsNeeded = 5 * level;
    speed =800;
    delay = 500;
    score = 0;
    moveTimer = Timer();
    lockDelayTimer = Timer();
    lockDelayResets = 0;
    maxLockDelayResets = 10;
    hasHeld = false;
    wasHeld = false;
    heldType = -1;

    //Start the fps timer
    moveTimer.start();
}

void Tetromino::spawn( bool held )
{
    //Generate grab bag if starting to empty
    if( nextBag.size() < 4 )
    {
        fillBag();
    }
    
    //If the held Tetromino is to spawn
    if( held )
    {
        int tempType = type;
        type = heldType;
        heldType = tempType;
    }
    //If the next tetromino is to spawn
    else
    {
        type = nextBag[ 0 ];
        nextBag.erase( nextBag.begin() );
    }
    
    //Set variables
    rotation = ROTATION_SPAWN;
    tetroTop = -1;
    tetroLeft = 3;
}

void Tetromino::handleEvent( SDL_Event &e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch ( e.key.keysym.sym )
        {
            //Move Tetromino left
            case SDLK_LEFT:
                move( -1 );
                break;
            //Move Tetromino right
            case SDLK_RIGHT:
                move( 1 );
                break;
            //Rotate Tetromino counterclockwise
            case SDLK_a:
                rotate( 1 );
                break;
            //Rotate Tetromino clockwise
            case SDLK_d:
                rotate( 0 );
                break;
            case SDLK_RETURN:
                spawn( false );
                break;
            case SDLK_DOWN:
                speed /= 2;
                break;
            case SDLK_s:
                hold();
                break;
        }
    }
    if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch ( e.key.keysym.sym )
        {
            case SDLK_DOWN:
                speed *= 2;
                break;
        }
    }
}

void Tetromino::logic()
{
    if( moveTimer.getTicks() >= speed )
    {
        fall();
        completeLines();
        moveTimer.start();
    }
}

void Tetromino::move( int offset )
{
    
    //Reset lock delay timer if it is started
    if( lockDelayTimer.isStarted() && lockDelayResets < maxLockDelayResets )
    {
        lockDelayTimer.start();
        lockDelayResets++;
    }
    if( !checkCollisions( tetroLeft + offset, tetroTop, rotation ) )
    {
        tetroLeft += offset;
    }
}

void Tetromino::rotate( bool counterclock )
{
    //Reset lock delay timer if it is started
    if( lockDelayTimer.isStarted() && lockDelayResets < maxLockDelayResets )
    {
        lockDelayTimer.start();
        lockDelayResets++;
    }
    
    //O-Tetromino does not rotate
    if( type != TETRO_O )
    {
        int potentialRotation = 0;
        //If counterclockwise
        if( counterclock )
        {
            if( rotation == ROTATION_SPAWN )
            {
                potentialRotation = ROTATION_3;
            }
            else
            {
                potentialRotation = rotation - 1;
        
            }
        }
        //If clockwise
        if( !counterclock )
        {
            if( rotation == ROTATION_SPAWN )
            {
                potentialRotation = ROTATION_1;
            }
            else
            {
                potentialRotation = rotation + 1;
            }
        }
        
        //Fix rotation value
        if( potentialRotation < 0 )
        {
            potentialRotation = ROTATION_SPAWN + potentialRotation;
        }
        else
        {
            potentialRotation = potentialRotation % 4;
        }
        
        if( !checkCollisions( tetroLeft, tetroTop, potentialRotation ) || tryKick( counterclock, potentialRotation ) )
        {
            rotation = potentialRotation;
        }
    }
}
bool Tetromino::tryKick( bool counterclock, int potentialRotation )
{
    //Kick success flag
    bool success = true;
    
    //Attempt to wall kick
    success = wallKick( counterclock, potentialRotation );
    
    //Perform floor kick if wall kick does not work
    if( !success )
    {
        success = true;
        
        tetroTop--;
        //Handle collisions
        if( checkCollisions( tetroLeft, tetroTop, potentialRotation ) )
        {
            //Try one left
            if( !checkCollisions( tetroLeft - 1 , tetroTop, potentialRotation) )
            {
                tetroLeft--;
            }
            //Try one right
            else if( !checkCollisions( tetroLeft + 1, tetroTop, potentialRotation ) )
            {
                tetroLeft++;
            }
            else
            {
                tetroTop++;
                success = false;
            }
        }
    }
    return success;
}
bool Tetromino::wallKick( bool counterclock, int potentialRotation )
{
    //Wall kick success flag
    bool success = true;
    
    //Counterclockwise rotations
    if( counterclock )
    {
        //Try one left
        if( !checkCollisions( tetroLeft - 1 , tetroTop, potentialRotation) )
        {
            tetroLeft--;
        }
        //Try one right
        else if( !checkCollisions( tetroLeft + 1, tetroTop, potentialRotation ) )
        {
            tetroLeft++;
        }
        //Try one down
        else if( !checkCollisions( tetroLeft, tetroTop + 1, potentialRotation ) )
        {
            tetroTop++;
        }
        //Try one down and one left
        else if( !checkCollisions( tetroLeft - 1, tetroTop + 1, potentialRotation ) )
        {
            tetroLeft--;
            tetroTop++;
        }
        //Try one down and one right
        else if( !checkCollisions( tetroLeft + 1, tetroTop + 1, potentialRotation ) )
        {
            tetroLeft++;
            tetroTop++;
        }
        //If all checks fail
        else
        {
            success = false;
        }
    }
    //Clockwise rotation
    else if( !counterclock )
    {
        //Try one right
        if( !checkCollisions( tetroLeft + 1 , tetroTop, potentialRotation) )
        {
            tetroLeft++;
        }
        //Try one left
        else if( !checkCollisions( tetroLeft - 1, tetroTop, potentialRotation ) )
        {
            tetroLeft--;
        }
        //Try one down
        else if( !checkCollisions( tetroLeft, tetroTop + 1, potentialRotation ) )
        {
            tetroTop++;
        }
        //Try one down and one right
        else if( !checkCollisions( tetroLeft + 1, tetroTop + 1, potentialRotation ) )
        {
            tetroLeft++;
            tetroTop++;
        }
        //Try one down and one left
        else if( !checkCollisions( tetroLeft - 1, tetroTop + 1, potentialRotation ) )
        {
            tetroLeft--;
            tetroTop++;
        }
        //If all checks fail
        else
        {
            success = false;
        }
    }
    
    //Return if successful
    return success;
}

void Tetromino::completeLines()
{
    int completedLines = 0;
    
    //Check if each row is completed
    for( int row = 0; row < board.size(); row++ )
    {
        //Line completed flag
        int lineCompleted = true;
        //Check each square in the row
        for( int column = 0; column < board[ 0 ].size(); column++ )
        {
            //If a square is not filled its not completed
            if( board[ row ][ column ] == BACKGROUND )
            {
                lineCompleted = false;
            }
        }
        
        //If the current line is completed
        if( lineCompleted )
        {
            board.erase( board.begin() + row );
            board.insert( board.begin(), std::vector< int > ( COLUMNS, BACKGROUND ) );
            completedLines++;
        }
    }
    //Update score and points
    switch ( completedLines ) {
        case 1:
            score += 100 * level;
            currentPoints += 1;
            break;
        case 2:
            score += 300 * level;
            currentPoints += 3;
            break;
        case 3:
            score += 500 * level;
            currentPoints += 5;
            break;
        case 4:
            score += 800 * level;
            currentPoints += 8;
            break;
    }
    
    //Increase level if enough points have been obtained
    if( currentPoints >= pointsNeeded )
    {
        level++;
        currentPoints = currentPoints - pointsNeeded;
        pointsNeeded = 5 * level;
        speed = ( speed * 2 ) / 3;
    }
}

bool Tetromino::checkCollisions( int potentialTetroLeft, int potentialTetroTop, int potentialRotation )
{
    //Collision flag
    bool collided = false;
    
    //Check collisions
    for( int row = 0; row < shapeArrays[ potentialRotation ][ type ].size(); row++ )
    {
        for( int col = 0; col < shapeArrays[ potentialRotation ][ type ][ 0 ].size(); col++ )
        {
            if( shapeArrays[ potentialRotation ][ type ][ row ][ col ] != BACKGROUND )
            {
                if( ( ( potentialTetroLeft + col ) * BRICK_LENGTH ) < 0 || ( ( potentialTetroLeft + col ) * BRICK_LENGTH + 1) >  ( COLUMNS * BRICK_LENGTH ) )
                {
                    collided = true;
                }
                else if ( ( ( potentialTetroTop + row ) * BRICK_LENGTH ) < 0  || ( ( potentialTetroTop + row ) * BRICK_LENGTH + 1 ) >  ( ROWS * BRICK_LENGTH ) )
                {
                    collided = true;
                }
                else if( shapeArrays[ potentialRotation ][ type ][ row ][ col ] != BACKGROUND )
                {
                    if( board[ potentialTetroTop + row ][ potentialTetroLeft + col ] != BACKGROUND )
                    {
                        collided = true;
                    }
                }
            }
        }
    }
    
    //Return if collided
    return collided;
}

void Tetromino::fall()
{
    if( !lockDelayTimer.isStarted() )
    {
        tetroTop++;
        if( checkCollisions( tetroLeft, tetroTop, rotation ) )
        {
            tetroTop--;
            lockDelayTimer.start();
        }
    }
    else
    {
        if( lockDelayTimer.getTicks() >= delay )
        {
            lockDelayTimer.stop();
            lockDelayResets = 0;
            tetroTop++;
            if( checkCollisions( tetroLeft, tetroTop, rotation ) )
            {
                tetroTop--;
                for( int row = 0; row < shapeArrays[ rotation ][ type ].size(); row++ )
                {
                    for( int col = 0; col < shapeArrays[ rotation ][ type ][ 0 ].size(); col++ )
                    {
                        if( shapeArrays[ rotation ][ type ][ row ][ col ] != BACKGROUND )
                        {
                            board[ tetroTop + row ][ tetroLeft + col ] = type;
                        }
                    }
                }
                spawn( false );
                wasHeld = false;
                checkOver();
            }
        }
    }
}

void Tetromino::render()
{
    renderBoard();
    renderScore();
    renderNext();
    renderHeld();
    renderTetromino();
}

void Tetromino::renderTetromino()
{
    //Get coordinates of top left of Tetromino Shape
    int topleftX = BOARD_LEFT + ( tetroLeft * BRICK_LENGTH );
    int topleftY = BOARD_TOP + ( tetroTop * BRICK_LENGTH );
    
    //Get Tetromino shape
    std::vector< std::vector< int > > shape = shapeArrays[ rotation ][ type ];
    
    //Clip rectangle for rendering
    SDL_Rect clip = { 0, 0, BRICK_LENGTH, BRICK_LENGTH };
    
    for( int i = 0 ; i < shape.size(); i++ )
    {
        for( int j = 0; j < shape[ 0 ].size(); j++ )
        {
            if( shape[ i ][ j ] != BACKGROUND )
            {
                //Determine sprite to render
                clip.x = ( type % 2 ) * BRICK_LENGTH;
                clip.y = ( type / 2 ) * BRICK_LENGTH;
                
                //Render the Tetromino
                gSpriteSheetTeture.render( ( topleftX + ( j * BRICK_LENGTH ) ), ( topleftY + ( i * BRICK_LENGTH ) ), &clip );
            }
        }
    }
}

void Tetromino::renderBoard()
{
    SDL_Rect clip = { 0, 0, BRICK_LENGTH, BRICK_LENGTH };
    for( int row = -1; row <= ROWS; row++)
    {
        for( int col = -1; col <= COLUMNS; col++ )
        {
            if( !( row == -1 || row == ROWS || col == -1 || col == COLUMNS ) )
            {
                //Determine clip of sprite sheet to render
                int value = board[ row ][ col ];
                clip.x = ( value % 2 ) * BRICK_LENGTH;
                clip.y = ( value / 2 ) * BRICK_LENGTH;
            }
            //Render entrance
            else if( row == -1 && 2 <= col && col <= 7 )
            {
                clip.y = 3 * BRICK_LENGTH;
                clip.x = BRICK_LENGTH;
                gSpriteSheetTeture.render( ( BOARD_LEFT + ( col * BRICK_LENGTH ) ), ( BOARD_TOP - BRICK_LENGTH ), &clip );
                
                if( col == 2)
                {
                    clip.x = 2 * BRICK_LENGTH;
                    gSpriteSheetTeture.render( ( BOARD_LEFT + ( col * BRICK_LENGTH ) ), ( BOARD_TOP - BRICK_LENGTH ), &clip );
                }
                else if( col == 7 )
                {
                    clip.x = 3 * BRICK_LENGTH;
                    gSpriteSheetTeture.render( ( BOARD_LEFT + ( col * BRICK_LENGTH ) ), ( BOARD_TOP - BRICK_LENGTH ), &clip );
                }
            }
            else
            {
                clip.x = 2 * BRICK_LENGTH;
                clip.y = 0;
                if( row == ROWS )
                    clip.y += 2 * BRICK_LENGTH;
                else if( row != -1 )
                    clip.y += BRICK_LENGTH;
                
                if( col == COLUMNS )
                    clip.x += 2 * BRICK_LENGTH;
                else if( col != -1)
                    clip.x += BRICK_LENGTH;
            }
            
            //Render the sprite
            gSpriteSheetTeture.render( ( BOARD_LEFT + ( col * BRICK_LENGTH ) ), ( BOARD_TOP + ( row * BRICK_LENGTH ) ), &clip );
        }
    }
}

void Tetromino::renderScore()
{
    SDL_Rect clip = { 0, 0, BRICK_LENGTH, BRICK_LENGTH };
    for( int col = -1; col <= SCORE_WIDTH; col++)
    {
        for( int row = -1; row <= SCORE_HEIGHT; row++ )
        {
            if( !( col == -1 || col == SCORE_WIDTH || row == -1 || row == SCORE_HEIGHT ) )
            {
                //Render the background
                clip.x = BRICK_LENGTH;
                clip.y = 3 * BRICK_LENGTH;
                gSpriteSheetTeture.render( ( SCORE_LEFT + ( col * BRICK_LENGTH ) ), ( SCORE_TOP + ( row * BRICK_LENGTH ) ), &clip );
                
                //Determine clip of sprite sheet to render
                if( row == 0 )
                {
                    clip.y = SCORE_START * BRICK_LENGTH;
                    clip.x = col * BRICK_LENGTH;
                }
                else
                {
                    int digit = 100000 / pow( 10, col );
                    int value = score % digit;
                    value = ( value * 10 ) / digit;
                    clip.x = ( value % 5 ) * BRICK_LENGTH;
                    clip.y = ( SCORE_START + 1 + ( value / 5 ) ) * BRICK_LENGTH;
                }
                
                //Render the score
                gSpriteSheetTeture.render( ( SCORE_LEFT + ( col * BRICK_LENGTH ) ), ( SCORE_TOP + ( row * BRICK_LENGTH ) ), &clip );
                
                //Render the background
                clip.x = BRICK_LENGTH;
                clip.y = 3 * BRICK_LENGTH;
                gSpriteSheetTeture.render( ( SCORE_LEFT + ( col * BRICK_LENGTH ) ), ( SCORE_TOP + ( ( row + SCORE_HEIGHT + 2 ) * BRICK_LENGTH ) ), &clip );
                
                //Determine clip of sprite sheet to render
                if( !( row == 1 && ( col == 0 || col == SCORE_WIDTH - 1 ) ) )
                {
                    if( row == 0 )
                    {
                        clip.y = ( SCORE_START + 3 ) * BRICK_LENGTH;
                        clip.x = col * BRICK_LENGTH;
                    }
                    else
                    {
                        int digit = 1000 / pow( 10, col - 1 );
                        int value = level % digit;
                        value = ( value * 10 ) / digit;
                        clip.x = ( value % 5 ) * BRICK_LENGTH;
                        clip.y = ( SCORE_START + 1 + ( value / 5 ) ) * BRICK_LENGTH;
                    }
                
                    //Render the score
                    gSpriteSheetTeture.render( ( SCORE_LEFT + ( col * BRICK_LENGTH ) ), ( SCORE_TOP + ( ( row + SCORE_HEIGHT + 2 ) * BRICK_LENGTH ) ), &clip );
                }
            }
            else
            {
                clip.x = 2 * BRICK_LENGTH;
                clip.y = 0;
                if( row == SCORE_HEIGHT )
                    clip.y += 2 * BRICK_LENGTH;
                else if( row != -1 )
                    clip.y += BRICK_LENGTH;
                
                if( col == SCORE_WIDTH )
                    clip.x += 2 * BRICK_LENGTH;
                else if( col != -1)
                    clip.x += BRICK_LENGTH;
                
                //Render the sprite
                gSpriteSheetTeture.render( ( SCORE_LEFT + ( col * BRICK_LENGTH ) ), ( SCORE_TOP + ( row * BRICK_LENGTH ) ), &clip );
                
                //Render the sprite
                gSpriteSheetTeture.render( ( SCORE_LEFT + ( col * BRICK_LENGTH ) ), ( SCORE_TOP + ( ( row + SCORE_HEIGHT + 2 ) * BRICK_LENGTH ) ), &clip );
            }
        }
    }
}

void Tetromino::renderNext()
{
    int nextHeight = 4;
    int nextWidth = 6;
    int nextLeft = ( SCREEN_WIDTH - ( ( nextWidth + 2 ) * BRICK_LENGTH ) ) / 6 * 5;
    int nextTop = 0;
    
    SDL_Rect clip = { 0, 0, BRICK_LENGTH, BRICK_LENGTH };
    
    //Render the next 3 Tetrominos
    for( int i = 0; i < 3; i++ )
    {
        nextTop = ( 4 * BRICK_LENGTH  ) + ( BRICK_LENGTH * ( i * ( nextHeight + 2 ) ) );
        int nextType = nextBag[ i ];
        int nextRotation = ROTATION_SPAWN;
        
        std::vector< std:: vector< int > > nextShape = shapeArrays[ nextRotation ][ nextType ];
        
        for( int col = -1; col <= nextWidth; col++ )
        {
            for( int row = -1; row <= nextHeight; row++ )
            {
                if( !( col == -1 || col == nextWidth || row == -1 || row == nextHeight ) )
                {
                    if( row == 0 || row == nextHeight - 1 || col == 0 || col == nextWidth - 1)
                    {
                        clip.x = BRICK_LENGTH;
                        clip.y = 3 * BRICK_LENGTH;
                    }
                    else
                    {
                        int value = nextShape[ row - 1 ][ col - 1 ];
                        clip.x = ( value % 2 ) * BRICK_LENGTH;
                        clip.y = ( value / 2 ) * BRICK_LENGTH;
                    }
                }
                else
                {
                    clip.x = 2 * BRICK_LENGTH;
                    clip.y = 0;
                    if( row == nextHeight )
                        clip.y += 2 * BRICK_LENGTH;
                    else if( row != -1 )
                        clip.y += BRICK_LENGTH;
                    
                    if( col == nextWidth )
                        clip.x += 2 * BRICK_LENGTH;
                    else if( col != -1)
                        clip.x += BRICK_LENGTH;
                }
                //Render the sprite
                gSpriteSheetTeture.render( ( nextLeft + ( col * BRICK_LENGTH ) ), ( nextTop + ( row * BRICK_LENGTH ) ), &clip );
            }
        }
    }
}

void Tetromino::renderHeld()
{
    int height = 4;
    int width = 6;
    int left = SCORE_LEFT - ( ( width - SCORE_WIDTH ) * BRICK_LENGTH );
    int top = ( SCORE_TOP + ( 2 * ( SCORE_HEIGHT + 2 ) * BRICK_LENGTH ) );
    
    SDL_Rect clip = { 0, 0, BRICK_LENGTH, BRICK_LENGTH };
    
    int rotation = ROTATION_SPAWN;
        
    std::vector< std::vector< std:: vector< int > > > spawnShapes = shapeArrays[ rotation ];
        
    for( int col = -1; col <= width; col++ )
    {
        for( int row = -1; row <= height; row++ )
        {
            if( !( col == -1 || col == width || row == -1 || row == height ) )
            {
                if( row == 0 || row == height - 1 || col == 0 || col == width - 1 || heldType == -1 )
                {
                    clip.x = BRICK_LENGTH;
                    clip.y = 3 * BRICK_LENGTH;
                }
                else
                {
                    int value = spawnShapes[ heldType ][ row - 1 ][ col - 1 ];
                    clip.x = ( value % 2 ) * BRICK_LENGTH;
                    clip.y = ( value / 2 ) * BRICK_LENGTH;
                }
            }
            else
            {
                clip.x = 2 * BRICK_LENGTH;
                clip.y = 0;
                if( row == height )
                    clip.y += 2 * BRICK_LENGTH;
                else if( row != -1 )
                    clip.y += BRICK_LENGTH;
                
                if( col == width )
                    clip.x += 2 * BRICK_LENGTH;
                else if( col != -1)
                    clip.x += BRICK_LENGTH;
            }
            //Render the sprite
            gSpriteSheetTeture.render( ( left + ( col * BRICK_LENGTH ) ), ( top + ( row * BRICK_LENGTH ) ), &clip );
        }
    }
}

void Tetromino::fillBag()
{
    //Generate a list of all pieces
    std::vector< int > allPieces;
    for( int i = 0; i < TETRO_TOTAL; i++ )
    {
        allPieces.push_back( i );
    }
    
    //Shuffle the list
    std::shuffle( std::begin( allPieces ), std::end( allPieces ) , *RNG );
    //Add to the bag
    for( int i = 0; i < allPieces.size(); i++ )
    {
        nextBag.push_back( allPieces[ i ] );
    }
}

void Tetromino::hold()
{
    if( !wasHeld )
    {
        if( hasHeld )
        {
            spawn( true );
        }
        else
        {
            heldType = type;
            hasHeld = true;
            spawn( false );
        }
        wasHeld = true;
    }
}

void Tetromino::checkOver()
{
    //Check if a block can not be spawned
    for( int col = 0; col < shapeArrays[ 0 ][ 0 ][ 0 ].size(); col++ )
    {
        if( shapeArrays[ ROTATION_SPAWN ][ type ][ 1 ][ col ] != BACKGROUND )
        {
            //If it collides the game has finished
            if( board[ 0 ][ 3 + col ] != BACKGROUND )
            {
                gameOver = true;
            }
            else
            {
                if( checkCollisions( tetroLeft, tetroTop + 1, rotation ) )
                {
                    gameOver = true;
                }
            }
        }
    }
    
    //If the game is over reset
    if( gameOver )
    {
        reset();
    }
}

void Tetromino::reset()
{
    //Game over flag
    gameOver = false;
    
    //Seeding the random number generator
    unsigned int seed = static_cast< int >( std::chrono::system_clock::now().time_since_epoch().count() );
    RNG = new std::mt19937( seed );
    
    //Initialize variables
    spawn( false );
    
    //Initialize the board
    for( int row = 0; row < ROWS; row++ )
    {
        for( int col = 0; col < COLUMNS; col++ )
        {
            board[ row ][ col ] = BACKGROUND;
        }
    }
    
    //Initialize game variables
    level = 1;
    currentPoints = 0;
    pointsNeeded = 5 * level;
    speed =800;
    delay = 500;
    score = 0;
    lockDelayResets = 0;
    maxLockDelayResets = 10;
    wasHeld = false;
    
    //Start the fps timer
    moveTimer.start();
    lockDelayTimer.stop();
}

std::vector< std::vector< int > > Tetromino::getBoard()
{
    return board;
}

////////// Other Methods //////////

bool init()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO  ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Set texture filtering to linear
            if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            {
                printf( "Warning: Linear texture filtering not enabled!" );
            }
            
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xff );
                
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                
                //Initialize SDL mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not be initialized! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
                
                int audioFlags = MIX_INIT_OGG;
                if( !( Mix_Init( audioFlags ) & audioFlags ) )
                {
                    printf( "SDL_mixer could initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
            }
        }
    }
    
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //Load spritesheet
    if( !gSpriteSheetTeture.loadFromFolder( "Sprites/SpriteSheet.png" ) )
    {
        printf( "Failed to load spritesheet texture!" );
        success = false;
    }
    
    //Load music
    gMusic = Mix_LoadMUS( "Music/ThemeA.ogg" );
    if( gMusic == NULL )
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    return success;
}

void close()
{
    //Free loaded textures
    gSpriteSheetTeture.free();
    
    //Free music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
    
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    //Exiit SDL subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, const char * argv[])
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;
            
            //Start music
            Mix_PlayMusic( gMusic, -1 );
            
            //Event handler
            SDL_Event e;
            
            //The tetromino handler
            Tetromino tetro;
            
            //While the application is running
            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //Requests exit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    
                    tetro.handleEvent( e );
                }
                
                //Handle movement and other logic
                tetro.logic();
                    
                //Render
                //Clear the screen
                SDL_SetRenderDrawColor( gRenderer, 0x60, 040, 0x60, 0xFF );
                SDL_RenderClear( gRenderer );
                
                //Render current Tetromino
                tetro.render();
                
                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    
    //Free resources and close SDL
    close();
    
    return 0;
}
