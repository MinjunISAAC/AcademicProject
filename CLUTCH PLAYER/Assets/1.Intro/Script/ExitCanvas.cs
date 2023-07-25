using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExitCanvas : MonoBehaviour
{
    public GameObject exitPopUp;

    void Start()
    {

    }

    void Update()
    {

    }

    public void ExitPopUpDown()
    {
        exitPopUp.SetActive(false);
    }
    public void ExitBtn()
    {
        Application.Quit();
    }

}
