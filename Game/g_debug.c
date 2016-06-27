FileContents GetLog(){
    char* fname = {"../DebugFile.dm"};

    return GetFileContents(fname);
}

void DebugRender(GameState* state){
	Renderer* renderer = state->renderer;
	PhysicsManager* physics = state->physics;

	FileContents file = GetLog();

	Maze* maze = state->maze;

	int size = maze->dims * maze->dims;

	char* grid = (char*)file.contents;

	maze->data = grid + (state->mazeFrame * size);

	int numTiles = InitMaze(physics, 11.0f, 11.0f, 	maze);
}