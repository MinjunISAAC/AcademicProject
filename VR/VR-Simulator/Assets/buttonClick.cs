using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;

public class buttonClick : MonoBehaviour
{
    public DigitalInput buttonValue;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(buttonValue == true)
        {
            Input.GetMouseButtonDown(0);
        }
    }
}
