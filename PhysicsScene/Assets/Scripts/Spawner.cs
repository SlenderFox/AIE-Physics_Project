using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawner : MonoBehaviour
{
    public GameObject prefab;
    private float timer = 0;

    // Update is called once per frame
    void Update ()
    {
        if (Input.GetKey(KeyCode.Space) && timer >= 0.2f)
        {
            GameObject newObj = Instantiate(prefab, transform.position, Quaternion.identity);
            Rigidbody rb = newObj.GetComponent<Rigidbody>();
            rb.AddForce(newObj.transform.forward * 500.0f);
            timer = 0;
        }
        timer += Time.deltaTime;
    }
}