using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class fireSoundManager : MonoBehaviour
{
    float timer;
    int waitingTime;

    private AudioSource musicPlayer;
    public AudioClip bombSound;


    void Start()
    {
        timer = 0.0f;
        waitingTime = 2;

        musicPlayer = GetComponent<AudioSource>();
        Invoke("playSound", 75f);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public  void  playSound()
    {
        musicPlayer.Stop();
        musicPlayer.clip = bombSound;
        musicPlayer.Play();
        Handheld.Vibrate();
    }
}
