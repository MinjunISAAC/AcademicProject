using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;

public class playerTouch : MonoBehaviour
{
    public DigitalInput buttonValue;
    // Start is called before the first frame update
    void Start()
    {
        buttonValue = GameObject.Find("ClickButton").GetComponent("DigitalInput") as DigitalInput;
    }

    // Update is called once per frame
    void Update()
    {
        if(buttonValue.Value == true)
        {
            Input.GetMouseButtonDown(0);
        }
    }
}
