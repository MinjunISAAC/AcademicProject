using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;

public class fireExEffect : MonoBehaviour
{
    public DigitalInput buttonValue;
    public fireCheck fireEx;
    public ParticleSystem ps;


    void Start()
    {
        buttonValue = GameObject.Find("ClickButton2").GetComponent("DigitalInput") as DigitalInput;
        fireEx = GameObject.Find("fire extinguisher").GetComponent("fireCheck") as fireCheck;
    }
        // Update is called once per frame
    void Update()
    {
        if((buttonValue.Value == true || Input.GetMouseButton(0)) && fireEx.check1 == true)
        {
            ps.Play();
        }
        else if (Input.GetMouseButton(0))
        {
            ps.Play();
        }
        else
        {
            ps.Stop();
        }
    }
}
