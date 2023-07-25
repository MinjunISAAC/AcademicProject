using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MenuBtn : MonoBehaviour
{
    public Button menuButton;
    public Button menuCancelButton;
    public GameObject menuPenel;

    void Start()
    {
        
    }

    public void open()
    {
        menuPenel.SetActive(true);
    }
    public void cancel()
    {
        menuPenel.SetActive(false);
    }
}
