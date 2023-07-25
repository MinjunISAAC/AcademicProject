using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RestartBtn : MonoBehaviour
{
    public LevelSetting levelValue;

    public coolTime timeCheck;

    public GameObject panel;

    public Button plusTime;
    void Start()
    {
        levelValue = GameObject.Find("GameManager").GetComponent<LevelSetting>();
        timeCheck = GameObject.Find("TimeChecker").GetComponent<coolTime>();
    }

    private void Update()
    {
        Debug.Log(int.Parse(levelValue.levelText.text));
    }
    public void quitBtn()
    {
        Application.Quit();
    }

    public void restartBtn()
    {
        levelValue.levelSet(int.Parse(levelValue.levelText.text));
        panel.SetActive(false);
        timeCheck.updateTime = 0f;
        timeCheck.canSlider = true;

        plusTime.interactable = true;
        timeCheck.plusTime = 0f;
    }

}
