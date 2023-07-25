using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class narrationText : MonoBehaviour
{
    public Text narration;
    private float time;
    private float time1;
    private float time2;

    public GameObject fire;

    public bool ck;
    public Button okConnection;

    public AudioSource theNarration;
    [SerializeField] private AudioClip[] clip;

    public windowWork window1;
    public windowWork window2;
    public windowWork window3;
    public windowWork window4;
    public windowWork window5;
    public windowWork window6;
    public windowWork window7;
    public windowWork window8;

    public ParticleSystem ps;

    void Start()
    {
        float alpha = 255f;

        ps = GetComponent<ParticleSystem>();

        narration.GetComponent<Text>().text = "  ";

        theNarration = GetComponent<AudioSource>();

        window1 = GameObject.Find("WindowUp1").GetComponent("windowWork") as windowWork;
        window2 = GameObject.Find("WindowUp3").GetComponent("windowWork") as windowWork;
        window3 = GameObject.Find("WindowUp5").GetComponent("windowWork") as windowWork;
        window4 = GameObject.Find("WindowUp7").GetComponent("windowWork") as windowWork;
        window5 = GameObject.Find("WindowUp9").GetComponent("windowWork") as windowWork;
        window6 = GameObject.Find("WindowUp11").GetComponent("windowWork") as windowWork;
        window7 = GameObject.Find("WindowUp13").GetComponent("windowWork") as windowWork;
        window8 = GameObject.Find("WindowUp15").GetComponent("windowWork") as windowWork;

        ck = false;
    }
    
    void Update()
    {

        time += Time.deltaTime;
        float timer = Mathf.Floor(time);
        float timer1 = Mathf.Floor(time1);
        float timer2 = Mathf.Floor(time2);


        textSet("  ");

        if (timer >= 1 && timer < 10)
        {
            textSet("안녕하세요. " + "\n" + "VR 재난안전 교육 프로그램을 진행하기에 앞서 컨트롤러와 블루투스 연결을 진행해주세요.");
        }
        else if (timer >= 10 && timer <= 17)
        {
            textSet("블루투스 연결은 정면의 Connect 버튼을 " + "\n" + "클릭하여 연결 가능합니다.");
        }
        else if (timer >= 27 && timer < 31)
        {
            textSet("블루투스와 컨트롤러의 연결이 완료되었습니다.");
        }
        else if (timer >= 31 && timer < 37)
        {
            textSet("뒤돌아 검지손가락으로 첫 번째 버튼을" + "\n" + "클릭하여 일정거리를 걸어보세요.");
        }
        else if (timer >= 41 && timer < 50.5f)
        {
            textSet("컨트롤러의 첫번째 버튼은 이동버튼. " + "\n" + "두번째 버튼은 이벤트 버튼입니다." + "\n" + "실험실의 앞쪽 오른쪽에 배치되어 있는 ");
        }
        else if (timer >= 50.5f && timer < 57)
        {
            textSet("소화기를 찾아 소화기를 응시하며" + "\n" + "두번째 버튼을 클릭 해보세요.");
        }
        else if (timer >= 63 && timer < 72)
        {
            textSet("왼쪽 소화기 사용 여부를 확인할 " + "\n" + "수 있는 UI의 색이 진하게 바뀌면 " + "\n" + "소화기를 사용할 수 있습니다. ");
        }
        else if (timer >= 78 && timer < 87.8f)
        {
            textSet("현재 실험실에 화재가 발생하였습니다. " + "\n" + "화재를 초기 진압하기 위해 화재 발생지역에서 " + "\n" + "1 ~ 2m 떨어진 곳에서 화재 발생지를 " + "\n" + "응시하고 두번째 버튼을 눌러보세요.");
        }
        else if (timer >= 87.8f && timer < 91)
        {
            textSet("응시하고 두번째 버튼을 눌러보세요.");
        }

        if (!fire)
        {
            time1 += Time.deltaTime;
        }

        Debug.Log("불 여부 : " + fire);

        Debug.Log("timer1 : " + timer1);

        if (timer1 == 1)
        {
            soundSet(6);
        }else if(timer1 == 21f)
        {
            soundSet(7);
        }
            
        if(timer1 >= 0.1 && timer1 < 10.7f)
        {
            textSet("화재 초기 진압을 완료하셨습니다. " + "\n" + "하지만 실험실 내부에 화재 연기가 자욱한 걸" + "\n" + " 확인하실 수 있습니다.");
        }
        else if (timer1 >= 10.7f && timer1 < 20.5f)
        {
            textSet("창문을 응시하시면 이벤트를 발생시킬 " + "\n" + "수 있는 창문이 있습니다. " + "\n" + "오른쪽 버튼을 사용하여 창문을 모두 개방하세요.");
        }
        else if (timer1 >= 22 && timer1 < 30)
        {
            textSet("또한 창문을 열기 위해서는 컨트롤러의 버튼 " + "\n" + "두 개를 동시에 클릭하여 소화기를 내려놓아야 합니다. ");
        }



        if (window1.windowOpenCheck == true && window2.windowOpenCheck == true && window3.windowOpenCheck == true && window4.windowOpenCheck == true &&
            window5.windowOpenCheck == true && window6.windowOpenCheck == true && window7.windowOpenCheck == true && window8.windowOpenCheck == true)
        {
            time2 += Time.deltaTime;
        }
        if(timer2 == 2)
        {
            soundSet(8);
        }
        if(timer2 >= 1 && timer2 < 14)
        {
            textSet("기본적인 화재 초기 조치를 완료하셨습니다." + "\n" + "하지만 초기 화재 진압에 실패하였을 경우" + "\n" + "휴대전화를 통하여 119에 화재사실을");
        }
        else if (timer2 >= 14 && timer2 < 17.5f)
        {
            textSet("빠르게 알린 후 신속히 대피하여야 합니다.");
        }


        if (timer == 0)
        {
            soundSet(0);
        }
        else if (timer == 10)
        {
            soundSet(1);
        }
        else if (timer == 25)
        {
            soundSet(2);
        }
        else if (timer == 40)
        {
            soundSet(3);
        }
        else if (timer == 63)
        {
            soundSet(4);
        }
        else if(timer == 76)
        {
            soundSet(5);
        }


    }

    public void textSet(string text)
    {
        narration.text = text;
    }

    public void soundSet(int i)
    {
        theNarration.clip = clip[i];
        theNarration.Play();
    }

    
}
