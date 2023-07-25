using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FadeIn : MonoBehaviour
{
    public Image panel;
    public GameObject canvas;
    float time = 0f;
    float F_time = 1f;

    public void Awake()
    {
        Fade();
    }

    public void Update()
    {
        Debug.Log("hey");
        if (panel.color.a <= 0)
        {
            canvas.SetActive(false);
        }
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
        while (alpha.a > 0f)
        {
            time += Time.deltaTime / F_time;
            alpha.a = Mathf.Lerp(1, 0, time);
            panel.color = alpha;
            yield return null;
        }

        time = 0f;

        yield return new WaitForSeconds(1f);
    }
}
