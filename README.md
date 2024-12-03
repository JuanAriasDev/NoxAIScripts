This files are only a part of the project, so they will not build properly. You can download the full version to test it in Steam.

NoxAICharacter - This is a base character created for different enemies in the game.

NoxAIController - This is a base controller created to have a basic control over the enemies.

PathfindingGrid - This is the Navmesh where the AI is able to move, some of its atributes can be modified from Unreal (Size, number of tiles, size of the tiles).

PathfinderComponent - This is a component attached to the enemy, it controls what tiles the AI character can move through and what tiles it should avoid. As well as it calculates the path that it should follow to reach the player.

RejectionCharacter - It is the actor definition of one of the enemies in the game.

RejectionController - The specific controller for the RejectionCharacter.

Services and Tasks - This are the definition of the actions and checkups of the rejection for the BehaviorTree, so the AI character do what it has to do when it has to do it.
