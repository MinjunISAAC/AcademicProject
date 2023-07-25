using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;

public class PlayerCameraControl : MonoBehaviour
{
    public GameObject player;

    //public Transform camTr;
    float x;
    public DigitalInput buttonValue;
    public float MoveSpeed = 10;

    void Start()
    {
        //camTr = Camera.main.GetComponent<Transform>();
    }

    // Update is called once per frame
    void Update()
    {
        var x = transform.rotation.eulerAngles.y;
        var lot_y = transform.position.y;
        Debug.Log(x);
        
       // transform.rotation = Quaternion.Euler(0, x, 0);
        
        if (Input.GetKey(KeyCode.UpArrow))
        {
            transform.Translate(Vector3.forward * MoveSpeed * Time.deltaTime);
            transform.position = new Vector3(transform.position.x, lot_y, transform.position.z);
        }

    }
}
