using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;

public class walkSound : MonoBehaviour
{
    public DigitalInput buttonValue1;

    private AudioSource walkSource;

    [SerializeField] private AudioClip clip;

    void Start()
    {
        walkSource = GetComponent<AudioSource>();
        buttonValue1 = GameObject.Find("ClickButton").GetComponent("DigitalInput") as DigitalInput;

    }

    // Update is called once per frame
    void Update()
    {
        if (buttonValue1.Value == true)
        {
            walkSource.mute = false;
        }
        else if(buttonValue1.Value == false)
        {
            walkSource.mute = true;
        }
    }



}
