// (C) 2009 christian.schladetsch@gmail.com

#ifndef WORLD_H_INCLUDED
#	define WORLD_H_INCLUDED

/// The state of the game world
///
///
struct World : Object
{
	typedef std::set<Object *> Objects;		///< set of dynamic objects that require update and draw each frame

private:
	Objects objects;
	GameTime game_time;
	Playfield *playfield;

	float time_last_impact;		///< the time that the last impact was added
	float min_impact_time;		///< minimum time between impacts

public:
	void Construct(int width, int height);
	~World();

	bool Update(GameTime);
	void Draw(Matrix const &);

	GameTime GetGameTime() const { return game_time; }
	Box GetBounds() const;
	Playfield *GetPlayfield() { return playfield; }
	SDL_Surface *GetSurface() const;
	Point GetMidPoint() const;

	/// reset the player and playfield
	void Clear();

	/// add a new impact object at the given location and size
	void AddImpact(int x, int y, float radius);

protected:
	void UpdateObjects();
	void CollisionDetection();
	void DeleteObjects();

	void DrawObjects(Matrix const &M);
};

#endif // WORLD_H_INCLUDED

//EOF
