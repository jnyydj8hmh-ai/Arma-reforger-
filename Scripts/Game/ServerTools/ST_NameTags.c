//------------------------------------------------------------------------------------------------
// ST_NameTags - muestra el nombre de los jugadores encima de su cabeza.
// Se ejecuta solo en los clientes (no en el servidor dedicado).
//------------------------------------------------------------------------------------------------
modded class SCR_BaseGameMode
{
	// ---- Ajustes ----
	protected const float ST_TAG_MAX_DISTANCE   = 60;     // metros a partir de los que no se ve el nombre
	protected const bool  ST_TAG_ONLY_ALLIES    = true;   // true = solo tu bando; false = todos
	protected const bool  ST_TAG_NEED_SIGHT     = true;   // true = no se ven a traves de paredes
	protected const bool  ST_TAG_SHOW_DISTANCE  = true;   // añade "(23 m)" al nombre
	protected const float ST_TAG_HEIGHT         = 2.05;   // altura sobre los pies (m)
	protected const float ST_TAG_SIZE           = 12;     // tamaño del texto
	protected const int   ST_TAG_COLOR_ALLY     = 0xFF66FF66; // ARGB verde
	protected const int   ST_TAG_COLOR_ENEMY    = 0xFFFF5050; // ARGB rojo

	//------------------------------------------------------------------------------------------------
	override void OnGameStart()
	{
		super.OnGameStart();

		// El servidor dedicado no tiene pantalla
		if (System.IsConsoleApp())
			return;

		GetGame().GetCallqueue().CallLater(ST_DrawNameTags, 0, true); // cada frame
	}

	//------------------------------------------------------------------------------------------------
	protected void ST_DrawNameTags()
	{
		IEntity localEnt = SCR_PlayerController.GetLocalControlledEntity();
		PlayerController localPc = GetGame().GetPlayerController();
		BaseWorld world = GetGame().GetWorld();
		if (!localEnt || !localPc || !world)
			return;

		int localId = localPc.GetPlayerId();
		Faction localFaction = ST_GetFaction(localEnt);

		vector cam[4];
		world.GetCurrentCamera(cam);
		vector camPos = cam[3];
		vector camDir = cam[2];

		PlayerManager pm = GetGame().GetPlayerManager();
		array<int> players = {};
		pm.GetPlayers(players);

		foreach (int id : players)
		{
			if (id == localId)
				continue;

			ChimeraCharacter character = ChimeraCharacter.Cast(pm.GetPlayerControlledEntity(id));
			if (!character)
				continue;

			CharacterControllerComponent ctrl = character.GetCharacterController();
			if (ctrl && ctrl.IsDead())
				continue;

			// Bando
			Faction faction = ST_GetFaction(character);
			bool isAlly = localFaction && faction && localFaction.IsFactionFriendly(faction);
			if (ST_TAG_ONLY_ALLIES && !isAlly)
				continue;

			// Distancia
			vector head = character.GetOrigin() + Vector(0, ST_TAG_HEIGHT, 0);
			float dist = vector.Distance(camPos, head);
			if (dist > ST_TAG_MAX_DISTANCE)
				continue;

			// Detras de la camara
			if (vector.Dot(head - camPos, camDir) <= 0)
				continue;

			// Linea de vision
			if (ST_TAG_NEED_SIGHT && !ST_HasLineOfSight(world, camPos, head, localEnt, character))
				continue;

			string text = pm.GetPlayerName(id);
			if (ST_TAG_SHOW_DISTANCE)
				text = string.Format("%1 (%2 m)", text, Math.Round(dist));

			int color = ST_TAG_COLOR_ENEMY;
			if (isAlly)
				color = ST_TAG_COLOR_ALLY;

			// ONCE = se dibuja solo este frame, por eso se llama cada frame
			DebugTextWorldSpace.Create(world, text,
				DebugTextFlags.CENTER | DebugTextFlags.FACE_CAMERA | DebugTextFlags.ONCE,
				head[0], head[1], head[2], ST_TAG_SIZE, color, 0x00000000);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected Faction ST_GetFaction(IEntity ent)
	{
		FactionAffiliationComponent fac = FactionAffiliationComponent.Cast(ent.FindComponent(FactionAffiliationComponent));
		if (!fac)
			return null;

		return fac.GetAffiliatedFaction();
	}

	//------------------------------------------------------------------------------------------------
	protected bool ST_HasLineOfSight(BaseWorld world, vector from, vector to, IEntity exclude, IEntity target)
	{
		TraceParam param = new TraceParam();
		param.Start = from;
		param.End = to;
		param.Flags = TraceFlags.WORLD | TraceFlags.ENTS;
		param.Exclude = exclude;
		param.LayerMask = EPhysicsLayerPresets.Projectile;

		float hit = world.TraceMove(param, null);
		if (hit >= 0.98)
			return true;

		// Si lo primero que toca es el propio jugador, se le ve
		IEntity hitEnt = param.TraceEnt;
		while (hitEnt)
		{
			if (hitEnt == target)
				return true;
			hitEnt = hitEnt.GetParent();
		}

		return false;
	}
}
