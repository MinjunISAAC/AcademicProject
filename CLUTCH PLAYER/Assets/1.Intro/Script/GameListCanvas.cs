using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
public class GameListCanvas : MonoBehaviour
{
    public GameObject turnBtn;
    public GameObject mainPopUp;

    public Image panel;
    public GameObject fadepanel;
    
    float time = 0f;
    float F_time = 1f;

    int turnNum = 0;

    void Update()
    {
        Debug.Log(panel.color.a);
        Debug.Log(fadepanel);

        if (turnNum == 1)
        {
            SceneManager.LoadScene("Roll a Ball2");
        }
    }

    public void turnPopUp()
    {
        turnBtn.SetActive(false);
        mainPopUp.SetActive(true);
    }

    public void turn2()
    {
        SceneManager.LoadScene("Space Invader");
    }
    public void GameScene(int i)
    {
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
        while (alpha.a <= 1f)
        {
            time += Time.deltaTime / F_time;
            alpha.a = Mathf.Lerp(0, 1, time);
            panel.color = alpha;
            yield return null;
            if(panel.color.a == 1)
            {
                turnNum = 1;
            }
        }
        yield return null;
        
    }
}
