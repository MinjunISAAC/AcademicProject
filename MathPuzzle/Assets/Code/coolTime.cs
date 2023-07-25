using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class coolTime : MonoBehaviour
{
    public Image coolTimeSlider;
    public Text coolTimeText;

    public bool canSlider;
    private float coolTimedownTime;

    public float updateTime;

    public GameObject restartPanel;

    public float playTime;

    public float plusTime = 0f;

    public Button timePlus;

    void Start()
    {
        timePlus.interactable = true;

        canSlider = true;
        updateTime = 0.0f;
        coolTimedownTime = 100.0f;
        playTime = 100.0f;
    }

    void Update()
    {
        if (canSlider)
        {
            updateTime = updateTime + Time.deltaTime;
            coolTimeSlider.fillAmount = 1.0f - (Mathf.SmoothStep(0, playTime + plusTime, updateTime / (coolTimedownTime + plusTime)) / (100 + plusTime));
            coolTimeText.color = Color.black;
            if ((playTime + plusTime) - (int)(updateTime) == 0)
            {
                coolTimeText.text = "STOP";
                coolTimeText.color = Color.red;

                restartPanel.SetActive(true);
            }
            else
            {
                coolTimeText.text = ((playTime + plusTime) - (int)(updateTime)).ToString();
            }

            if (updateTime > (coolTimedownTime + plusTime))
            {
                coolTimeSlider.fillAmount = 0.0f;
                updateTime = 0.0f;
                canSlider = false;
            }
        }
    }

    public void plusTimeBtn()
    {
        plusTime = 30.0f;
        timePlus.interactable = false;
    }
}
