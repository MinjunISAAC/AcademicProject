using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using System;

public class timer : MonoBehaviour
{
    private Text textClock;
    private float time;

    void Start()
    {
        textClock = GetComponent<Text>();   
    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;
        float timer = Mathf.Floor(time);

        if (timer < 10)
        {
            textClock.text = "00" + ":" + "0" + timer.ToString();
        }
        else if (timer >= 10 && timer < 60)
        {
            textClock.text = "00" + ":" + timer.ToString();
        }
        else if (timer >= 60 && timer < 70)
        {
            timer = timer - 60;
            textClock.text = "01" + ":" + "0" + timer.ToString();
        }
        else if (timer < 120 && timer >= 70)
        {
            timer = timer - 60;
            textClock.text = "01" + ":" + timer.ToString();
        }        
        else if (timer < 130 && timer >= 120)
        {
            timer = timer - 120;
            textClock.text = "02" + ":" + "0" + timer.ToString();
        }
        else if (timer < 180 && timer >= 130)
        {
            timer = timer - 120;
            textClock.text = "02" + ":" + timer.ToString();
        }
        else if (timer < 190 && timer >= 180)
        {
            timer = timer - 180;
            textClock.text = "03" + ":" + "0" + timer.ToString();
        }
        else if (timer < 240 && timer >= 190)
        {
            timer = timer - 180;
            textClock.text = "03" + ":" +  timer.ToString();
        }
        else if (timer < 250 && timer >= 240)
        {
            timer = timer - 240;
            textClock.text = "04" + ":" + "0" + timer.ToString();
        }
        else if (timer < 300 && timer >= 250)
        {
            timer = timer - 240;
            textClock.text = "04" + ":" + timer.ToString();
        }
        else if (timer == 300)
        {
            textClock.text = "05" + ":" + "00";
        }

    }

}
