using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;

public class fireSound : MonoBehaviour
{
    public DigitalInput buttonValue;
    public DigitalInput buttonValue1;

    private AudioSource fireSource;

    [SerializeField] private AudioClip clip;

    public objectUsing fireUsing;

    public sigleButton fireCheckValue; 

    void Start()
    {
        fireSource = GetComponent<AudioSource>();
        buttonValue = GameObject.Find("ClickButton").GetComponent("DigitalInput") as DigitalInput;
        buttonValue1 = GameObject.Find("ClickButton2").GetComponent("DigitalInput") as DigitalInput;
        fireUsing = GameObject.Find("fireExCheck").GetComponent("objectUsing") as objectUsing;

        fireCheckValue = GameObject.Find("Main Camera").GetComponent("sigleButton") as sigleButton;
    }

    // Update is called once per frame
    void Update()
    {
        if (buttonValue1.Value == true && fireUsing.check == true)
        {
            fireSource.mute = false;
        }
        else if (buttonValue1.Value == false && fireCheckValue.collObjValue.check1 == true)
        {
            fireSource.mute = true;
        }
        if (buttonValue.Value == true && buttonValue1.Value == true)
        {
            fireUsing.check = false;
            fireSource.mute = true;
        }
    }
}
