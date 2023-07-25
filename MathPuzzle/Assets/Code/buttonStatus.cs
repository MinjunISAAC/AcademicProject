using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class buttonStatus : MonoBehaviour
{
    public int status;

    float time;

    void Start()
    {
    }

    void Update()
    {
        if (status == 1)
        {
            if (time < 0.5f)
            {
                GetComponent<Image>().color = new Color(1, 1, 1, 1 - time);
            }
            else
            {
                GetComponent<Image>().color = new Color(1, 1, 1, time);
                if (time > 1f)
                {
                    time = 0;
                }
            }
            time += Time.deltaTime;
            Debug.Log(time);
        }
        else if(status == 2)
        {
            GetComponent<Image>().color = new Color(1, 1, 1, 1);
        }

    }
}