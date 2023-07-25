using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SoundManage : MonoBehaviour
{
    public AudioSource back;

    void Start()
    {
        
    }
    public void backSound()
    {
        if (back.mute == true)
        {
            back.mute = false;
        }
        else
        {
            back.mute = true;
        }
    }
}
