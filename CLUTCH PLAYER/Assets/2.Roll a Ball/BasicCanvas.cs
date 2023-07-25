using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BasicCanvas : MonoBehaviour
{
    public GameObject Canvas1;
    public GameObject Canvas2;
    public GameObject Canvas3;
    public GameObject Canvas4;
    public GameObject Canvas5;
    public GameObject Canvas6;

    public GameObject gamestartBtn;

    public bool blueToothe;

    public Image panel;
    float time = 0f;
    float F_time = 1f;

    void Start()
    {
        blueToothe = false;
    }

    void Update()
    {
    }

    public void gameBtnOn()
    {
        gamestartBtn.SetActive(true);
    }
    public void turnCanvas12()
    {
        Canvas1.SetActive(false);
        Canvas2.SetActive(true);
    }
    public void turnCanvas21()
    {
        Canvas1.SetActive(true);
        Canvas2.SetActive(false);
    }
    public void turnCanvas23()
    {
        Canvas2.SetActive(false);
        Canvas3.SetActive(true);
    }
    public void turnCanvas32()
    {
        Canvas2.SetActive(true);
        Canvas3.SetActive(false);
    }
    public void turnCanvas34()
    {
        Canvas3.SetActive(false);
        Canvas4.SetActive(true);
    }

    public void blueToothOk()
    {
        blueToothe = true;
    }

    public void gameStartSence()
    {
        Canvas5.SetActive(false);
        Canvas6.SetActive(true);
        Fade();
    }

    public void Fade()
    {
        StartCoroutine(FadeFlow());
    }
    IEnumerator FadeFlow()
    {
        panel.gameObject.SetActive(true);
        time = 0f;
        Color alpha = panel.color;
        time = 0f;

        yield return new WaitForSeconds(1f);

        while (alpha.a > 0f)
        {
            time += Time.deltaTime / F_time;
            alpha.a = Mathf.Lerp(1, 0, time);
            panel.color = alpha;
            yield return null;
        }
        panel.gameObject.SetActive(false);
        yield return null;
    }
}
