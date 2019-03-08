using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class CannonBall : MonoBehaviour
{
    public float forceOnFire = 9000;
    bool fire = false;
    bool canFire = true;
    Rigidbody _rigidbody = null;

    private void Awake()
    {
        _rigidbody = GetComponent<Rigidbody>();
        _rigidbody.isKinematic = true;
    }

    // Update is called once per frame
    void Update ()
    {
        if (Input.anyKeyDown && canFire)
        {
            _rigidbody.isKinematic = false;
            _rigidbody.AddForce(transform.right * forceOnFire);
            canFire = false;
        }
    }
}