using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonAudio : MonoBehaviour
{
    public AudioSource button1;

    void Start()
    {
        
    }

    public void button1play()
    {
        button1.Play();
    }
}
