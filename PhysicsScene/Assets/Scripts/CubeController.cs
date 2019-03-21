using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CubeController : MonoBehaviour
{
    public Material awakeMaterial = null;
    public Material sleepMaterial = null;

    public Rigidbody rb;

    public float detectDistance = 1.0f;
    public float speed = 5.0f;

	// Use this for initialization
	void Start ()
    {
		
	}

    private void FixedUpdate()
    {
        // Moves the box around
        if (Input.GetKey(KeyCode.W))
        {
            rb.AddForce(transform.forward * speed);
        }
        if (Input.GetKey(KeyCode.S))
        {
            rb.AddForce(-transform.forward * speed);
        }
        if (Input.GetKey(KeyCode.D))
        {
            rb.AddForce(transform.right * speed);
        }
        if (Input.GetKey(KeyCode.A))
        {
            rb.AddForce(-transform.right * speed);
        }
    }

    // Update is called once per frame
    void Update ()
    {
        //Tests if the player is in the air or not
        Ray boxDetectRay = new Ray(transform.position + new Vector3(0, 0, 0.5f), new Vector3(0, 0, 1));
        //Debug.DrawLine(boxDetectRay.origin, boxDetectRay.origin + (Vector3.forward * detectDistance));
        RaycastHit boxDetectRayInfo;
        if (Physics.Raycast(boxDetectRay, out boxDetectRayInfo, detectDistance) && boxDetectRayInfo.collider.name.Contains("StaticCube"))
        {
            //Debug.Log("Found the cube!");
            GetComponent<MeshRenderer>().material = awakeMaterial;
        }
        else
        {
            GetComponent<MeshRenderer>().material = sleepMaterial;
        }
    }
}
