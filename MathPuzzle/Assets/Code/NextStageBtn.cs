using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NextStageBtn : MonoBehaviour
{
    public GameObject finishPanel;

    public LevelSetting levelValue;
    void Start()
    {
        levelValue = GameObject.Find("GameManager").GetComponent<LevelSetting>();
    }

    public void nextStage()
    {
        finishPanel.SetActive(false);

        if(int.Parse(levelValue.levelText.text) <= 100)
        {
            levelValue.levelSet(int.Parse(levelValue.levelText.text) + 1);
        }
        if (int.Parse(levelValue.levelText.text) > 100 && int.Parse(levelValue.levelText.text) <= 200)
        {
            levelValue.levelSet2(int.Parse(levelValue.levelText.text) + 1);
        }
    }

    public void quitBtn()
    {
        Application.Quit();
    }
}
