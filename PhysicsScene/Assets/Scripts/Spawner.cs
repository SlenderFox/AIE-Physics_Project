using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawner : MonoBehaviour
{
    public GameObject prefab;
    private float timer = 0;

    // Use this for initialization
    void Start ()
    {
    }

    // Update is called once per frame
    void Update ()
    {
        if (Input.anyKey == true && timer >= 0.2f)
        {
            GameObject newObj = Instantiate(prefab, transform.position, Quaternion.identity);
            Rigidbody rb = newObj.GetComponent<Rigidbody>();
            rb.AddForce(newObj.transform.forward * 500.0f);
            timer = 0;
        }
        timer += Time.deltaTime;
    }
}