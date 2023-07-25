using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainCanvas : MonoBehaviour
{
    public GameObject exitPopUp;
    public GameObject gameListPopUp;
    public GameObject mainPopUp;
    public GameObject soundOn;
    public GameObject soundOff;

    public AudioSource backGroundSound;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void ExitBtn()
    {
        exitPopUp.SetActive(true);
    }

    public void GameList()
    {
        gameListPopUp.SetActive(true);
        mainPopUp.SetActive(false);
    }

    public void SoundOn()
    {
        soundOff.SetActive(true);
        soundOn.SetActive(false);
        backGroundSound.mute = true;
    }

    public void SoundOff()
    {
        soundOff.SetActive(false);
        soundOn.SetActive(true);
        backGroundSound.mute = false;
    }
}
