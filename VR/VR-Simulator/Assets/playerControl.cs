using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;


public class playerControl : MonoBehaviour
{
    public GameObject playerCamera;
    public DigitalInput buttonValue;

    // Start is called before the first frame update
    void Start()
    {
        playerCamera = GameObject.Find("Main Camera");
        buttonValue = GameObject.Find("ClickButton").GetComponent("DigitalInput") as DigitalInput;
    }

    // Update is called once per frame
    void Update()
    {

        if (buttonValue.Value == true)
        {
            transform.position = playerCamera.transform.position;
            transform.rotation = Quaternion.Euler(transform.rotation.x, playerCamera.transform.rotation.y, transform.rotation.z);
        }
        else { }
    }
}
