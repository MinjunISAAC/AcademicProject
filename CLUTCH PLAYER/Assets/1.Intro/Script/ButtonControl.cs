using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonControl : MonoBehaviour
{
    public AudioSource btnSound;
    public AudioClip Clip1;

    public void BtnSound()
    {
        btnSound.PlayOneShot(Clip1);
    }
}