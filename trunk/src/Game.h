// (C) 2009 christian.schladetsch@gmail.com

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Phase.h"

/// @brief the toplevel game object.
///
/// stores the object factory for all other objects in the system.
/// manages the phase transitions and controls the main processing loop
struct Game
{
	Factory *factory;		///< source of all game objects: static and dynamic
	GameTime time;

	SDL_Surface *screen;

	bool initialised;
	bool finished;

	Phase::Base *phase;			///< the current phase of the game
	Phase::Base *next_phase;	///< the phase to transition to
	Time transition_ends;		///< when the current transition ends

	Font *font;

	World *world;

public:
	Game(int width, int height);
	~Game();

	/// create a new object of type T using the factorys
	template <class T>
	T *New()
	{
		T *obj = factory->template New<T>();
		obj->game = this;
		return obj;
	}

	/// top-level update method
	void Update();

	/// top-level draw method
	void Draw();

	/// returns true if the application is intending to close
	bool Finished() const { return finished; }

	/// returns true if all subsystems initialised correctly
	bool Initialised() const { return initialised; }

	/// returns the default font
	Font *GetFont() const { return font; }

	/// returns the game-time structure
	GameTime GetTime() const;

	/// returns the time at the start of this frame
	Time TimeNow() const;

	SDL_Surface *GetSurface() const;
	Color MakeColor(int r, int g, int b) const;

	/// move to a new phase
	void PhaseChange(Phase::Base *next_phase, Time transition_time = 0);

	void SetWorld(World *W) { world = W; }
	World *GetWorld() const { return world; }

protected:
	void ParseInput();

	/// deal with intention of player to move in the given direction
	void PlayerDirects(Direction);

	/// player stops wanting the given direction
	void PlayerUnDirects(Direction);

	/// returns true iff the game is currently moving between phases
	bool Transitioning() const;

	/// perform transition between levels
	void Transist();

	/// end the current transition now
	void EndTransition();

private:
	/// register game types with the object factory
	void RegisterTypes();

	/// prepare the SDL system
	bool InitialiseSDL(int width, int height);
};

#endif // GAME_H_INCLUDED

//EOF
