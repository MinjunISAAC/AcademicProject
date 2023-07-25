using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;


public class cameraControl : MonoBehaviour
{
    public Transform player;

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.UpArrow))
        {
            Vector3 position = player.position;
            transform.SetPositionAndRotation(position, Quaternion.identity);
        }
    }
}
