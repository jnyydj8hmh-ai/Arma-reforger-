//------------------------------------------------------------------------------------------------
// ServerTools - registro de jugadores y estado del servidor para Arma Reforger
// Se engancha al modo de juego base (SCR_BaseGameMode) mediante "modded class".
// Solo se ejecuta en el servidor (o en partidas locales / Workbench).
//------------------------------------------------------------------------------------------------
modded class SCR_BaseGameMode
{
	// Intervalo del informe de estado (milisegundos)
	protected const int ST_STATUS_INTERVAL_MS = 5 * 60 * 1000; // 5 minutos

	protected bool m_bST_TimerStarted;
	protected ref map<int, int> m_mST_SpawnCount = new map<int, int>();

	//------------------------------------------------------------------------------------------------
	override void OnPlayerConnected(int playerId)
	{
		super.OnPlayerConnected(playerId);

		if (!Replication.IsServer())
			return;

		string name = GetGame().GetPlayerManager().GetPlayerName(playerId);
		Print(string.Format("[ServerTools] Jugador conectado: %1 (id %2)", name, playerId), LogLevel.NORMAL);

		m_mST_SpawnCount.Set(playerId, 0);

		if (!m_bST_TimerStarted)
		{
			m_bST_TimerStarted = true;
			GetGame().GetCallqueue().CallLater(ST_PrintStatus, ST_STATUS_INTERVAL_MS, true);
		}
	}

	//------------------------------------------------------------------------------------------------
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		super.OnPlayerSpawned(playerId, controlledEntity);

		if (!Replication.IsServer())
			return;

		int count = m_mST_SpawnCount.Get(playerId) + 1;
		m_mST_SpawnCount.Set(playerId, count);

		string name = GetGame().GetPlayerManager().GetPlayerName(playerId);
		vector pos = controlledEntity.GetOrigin();
		Print(string.Format("[ServerTools] %1 ha reaparecido (vez %2) en %3", name, count, pos), LogLevel.NORMAL);
	}

	//------------------------------------------------------------------------------------------------
	protected void ST_PrintStatus()
	{
		PlayerManager pm = GetGame().GetPlayerManager();
		array<int> players = {};
		pm.GetPlayers(players);

		Print(string.Format("[ServerTools] Estado: %1 jugador(es) conectado(s)", players.Count()), LogLevel.NORMAL);
		foreach (int id : players)
		{
			Print(string.Format("[ServerTools]   - %1 (id %2, reapariciones: %3)", pm.GetPlayerName(id), id, m_mST_SpawnCount.Get(id)), LogLevel.NORMAL);
		}
	}
}
