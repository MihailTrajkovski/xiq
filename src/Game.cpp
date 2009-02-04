// (C) 2009 christian.schladetsch.net

#include "PhaseCommon.h"

#include "Xiq.h"
#include "Styx.h"
#include "Impact.h"
#include "Font.h"

Game::Game(int width, int height)
{
	phase = 0;
	world = 0;
//	level = 0;
//	world = 0;
//	player = 0;
	screen = 0;
	factory = 0;
	next_phase = 0;
	font = 0;

	initialised = false;

	if (!InitialiseSDL(width, height))
		return;

	factory = new Factory();

	RegisterTypes();

//	player = New<Player>();
//
//	world = New<World>();
//	world->Construct(width, height);

	font = new Font("font");

	PhaseChange(New<Phase::Boot>());

	initialised = true;
	finished = false;
}

void Game::RegisterTypes()
{
	factory->AddClass<Playfield>();
	factory->AddClass<Player>();
	factory->AddClass<Xiq>();
	factory->AddClass<Styx>();
	factory->AddClass<Level>();
	factory->AddClass<World>();

	factory->AddClass<Phase::Boot>();
	factory->AddClass<Phase::Attract>();
	factory->AddClass<Phase::Play>();
}

Game::~Game()
{
	Delete(phase);
	Delete(next_phase);
//	Delete(player);
//	Delete(level);
//	Delete(world);
}

//World *Game::GetWorld() const
//{
//	return world;
//}

SDL_Surface *Game::GetSurface() const
{
	return screen;
}

GameTime Game::GetTime() const
{
	return time;
}

Color Game::MakeColor(int r, int g, int b) const
{
	return SDL_MapRGB(GetSurface()->format, r, g, b);
}

Time Game::TimeNow() const
{
	return time.TotalElapsedSeconds();
}

bool Game::Transitioning() const
{
	return next_phase != 0 && time.IsAfter(transition_ends);
}

void Game::Transist()
{
	if (time.IsAfter(transition_ends))
	{
		EndTransition();
	}
}

void Game::EndTransition()
{
	Delete(phase);
	phase = next_phase;
	next_phase = 0;
	transition_ends = 0;
}

void Game::PhaseChange(Phase::Base *next, Time transition_time)
{
	if (!next)
		return;
	if (Transitioning())
		EndTransition();
	transition_ends = TimeNow() + transition_time;
	next_phase = next;
}

//void Game::InitialiseLevel()
//{
//	Styx *styx = new Styx();
//	styx->SetWorld(this);
//
//	float radius_mul = current_level*0.5f;
//
//	Point spawn_point(0,0);
//	styx->SetLocation(spawn_point);
//	styx->SetRadius(15*radius_mul);
//	styx->SetDirection(Direction::Down);
//	styx->SetSpeed(40);
//	styx->SetColor(MakeColor(255,0,0));
//	objects.insert(styx);
//
//	if (current_level > 1)
//	{
//
//		Styx *styx2 = new Styx();
//		styx2->SetWorld(this);
//
//		styx2->SetLocation(Point(GetPlayfield().GetWidth() - 1,0));
//		styx2->SetRadius(8*radius_mul);
//		styx2->SetDirection(Direction::Down);
//		styx2->SetSpeed(100);
//		styx2->SetColor(MakeColor(100,0,250));
//
//		objects.insert(styx2);
//	}
//	if (current_level > 2)
//	{
//
//		Styx *styx2 = new Styx();
//		styx2->SetWorld(this);
//
//		styx2->SetLocation(Point(GetPlayfield().GetWidth() - 1, playfield.GetHeight() - 1));
//		styx2->SetRadius(22*radius_mul);
//		styx2->SetDirection(Direction::Left);
//		styx2->SetSpeed(100);
//		styx2->SetColor(MakeColor(50,255,250));
//
//		objects.insert(styx2);
//	}
//
//}

void Game::Update()
{
	time.StartFrame();
	ParseInput();
	if (Transitioning())
	{
		Transist();
	}
	else if (phase)
	{
		phase->Update(time);
	}
	factory->Purge();
}

void Game::ParseInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			finished = true;
		if (event.type == SDL_QUIT)
			finished = true;
		if (!phase)
			continue;
		bool handled = phase->InputEvent(event);
		if (handled)
			continue;
	}
}

void Game::Draw()
{
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 30, 30, 50));
	if (phase)
		phase->Draw(Matrix());
	SDL_Flip(screen);
}

bool Game::InitialiseSDL(int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return false;
    }

    // make sure SDL clseans up before exit
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(width, height, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (!screen)
    {
        printf("Unable to create window: %s\n", SDL_GetError());
        return false;
    }

	SDL_WM_SetCaption("XIQ != QIX", "XIQ");
	return true;
}

Playfield *Object::GetPlayfield() const
{
	return GetRoot()->GetWorld()->GetPlayfield();
}

//EOF
