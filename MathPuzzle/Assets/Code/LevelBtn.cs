using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LevelBtn : MonoBehaviour
{
    public GameObject level_1;
    public GameObject level_2;
    public GameObject level_3;
    public GameObject level_4;
    public GameObject level_5;

    public GameObject LevelPopup;
    public GameObject LevelPopup2;
    void Start()
    {
        level_1.SetActive(true);
        level_2.SetActive(false);
        level_3.SetActive(false);
        level_4.SetActive(false);
        level_5.SetActive(false);
    }

    public void level1()
    {
        level_1.SetActive(true);
        level_2.SetActive(false);
        level_3.SetActive(false);
        level_4.SetActive(false);
        level_5.SetActive(false);
    }
    public void level2()
    {
        level_1.SetActive(false);
        level_2.SetActive(true);
        level_3.SetActive(false);
        level_4.SetActive(false);
        level_5.SetActive(false);
    }
    public void level3()
    {
        level_1.SetActive(false);
        level_2.SetActive(false);
        level_3.SetActive(true);
        level_4.SetActive(false);
        level_5.SetActive(false);
    }
    public void level4()
    {
        level_1.SetActive(false);
        level_2.SetActive(false);
        level_3.SetActive(false);
        level_4.SetActive(true);
        level_5.SetActive(false);
    }
    public void level5()
    {
        level_1.SetActive(false);
        level_2.SetActive(false);
        level_3.SetActive(false);
        level_4.SetActive(false);
        level_5.SetActive(true);
    }
    public void levelPopup()
    {
        level_1.SetActive(true);
        level_2.SetActive(false);
        level_3.SetActive(false);
        level_4.SetActive(false);
        level_5.SetActive(false);

        LevelPopup.SetActive(false);
    }
    public void levelPopup2()
    {
        LevelPopup2.SetActive(true);
    }
}
