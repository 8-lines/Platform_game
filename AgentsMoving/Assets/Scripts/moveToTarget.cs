using UnityEngine;
using System.Collections;
using UnityEngine.AI;
using System.Threading;

public class moveToTarget : MonoBehaviour {
	
	public NavMeshAgent agent;
	public GameObject target1;
	public GameObject target2;
	public GameObject target3;
	private GameObject newTarget;
	private GameObject prevTarget;


	bool stop;
	private int count;
	private bool isCollided;
	private float spd;

	void Start () {
		agent = GetComponent<NavMeshAgent>();
		count = 1;
		isCollided = false;
		stop = false;
		spd = agent.speed;

		var color = new Color(Random.Range(0f, 1), Random.Range(0f, 1), Random.Range(0f, 1));
		GetComponent<Renderer>().material.color = color;
		var color2 = color;
		color2.b = 1;
		target1.GetComponent<Renderer>().material.color = color2;
		target2.GetComponent<Renderer>().material.color = color2;
		target3.GetComponent<Renderer>().material.color = color2;
		agent.destination = target1.transform.position;


	}

	void Update () 
	{
		if (agent.isOnNavMesh && agent.remainingDistance < 0.01) 
		{
			if ((count == 1) && (isCollided == false))
			{
				prevTarget = target1;
				newTarget = target2;
				count++;
				isCollided = true;
			}

			if ((count == 2) && (isCollided == false))
			{
				prevTarget = target2;
				newTarget = target3;
				count++;
				isCollided = true;
			}

			if ((count == 3) && (isCollided == false))
			{
				prevTarget = target3;
				newTarget = target1;
				count = 1;
				isCollided = true;
			}
				
				var to = prevTarget.transform;
				var hit = new NavMeshHit ();
				var pos = to.position + new Vector3 (Random.Range (-10.0f, 10.0f), 0, Random.Range (-10.0f, 10.0f));
				NavMesh.SamplePosition (pos, out hit, 100, 1);
				Debug.Log (hit.position);
				prevTarget.transform.position = hit.position + new Vector3 (0, 0.45f, 0);
				
				agent.destination = newTarget.transform.position;

				isCollided = false;
				stop = true;
				agent.speed = 0;
				
		}

		if (stop == true) 
		{
			stop = false;
			StartCoroutine (Delay ());
		}

							
	}

	IEnumerator Delay ()
	{
		yield return new WaitForSeconds(2);
		agent.speed = spd;

	}	
}
