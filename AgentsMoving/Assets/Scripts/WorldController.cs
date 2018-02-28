using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class WorldController : MonoBehaviour
{
	[System.Serializable]
	public class AgentSettings
	{
		public float radius;
		public float climbAngle;
		public float jump;
	}
	private NavMeshData data;
	public List<AgentSettings> settings;
	public bool act = false;
	Bounds area = new Bounds();

	void Start()
	{
		data = new NavMeshData();
		NavMesh.AddNavMeshData(data);
	}

	public void RebakeMesh()
	{
		area = new Bounds();

		List<NavMeshBuildSource> sources = new List<NavMeshBuildSource>();
		foreach (Transform go in transform)
		{
			var src = new NavMeshBuildSource();
			src.transform = go.localToWorldMatrix;
			src.shape = NavMeshBuildSourceShape.Mesh;
			src.sourceObject = go.GetComponent<MeshFilter>().mesh;
			sources.Add(src);

			area.Encapsulate(go.GetComponent<Renderer>().bounds);
		}
		area.Expand(1);

		List<NavMeshBuildSettings> configs = new List<NavMeshBuildSettings>();
		foreach (var agent in settings)
		{
			var s = NavMesh.CreateSettings();
			s.agentClimb = agent.climbAngle;
			s.agentRadius = agent.radius;
			s.agentSlope = agent.jump;
			s.agentTypeID = 1;
			configs.Add(s);
		}

		NavMeshBuilder.UpdateNavMeshData(data, configs[0], sources, area);
	}
}