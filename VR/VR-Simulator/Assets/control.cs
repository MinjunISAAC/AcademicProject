using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;
using UnityEngine.EventSystems;

public class control : MonoBehaviour
{
    public DigitalInput buttonValue;

    public Camera cam; //메인카메라
    private float speed = 0.5f; // 이동속도

    private AudioSource walkSource;

    [SerializeField] private AudioClip clip;

    void Start()
    {
        buttonValue = GameObject.Find("ClickButton").GetComponent("DigitalInput") as DigitalInput;
    }

    void Update()
    {

        //메인카메라가 바라보는 방향입니다.
        Vector3 dir = cam.transform.localRotation * Vector3.forward;
        //카메라가 바라보는 방향으로 팩맨도 바라보게 합니다.
        transform.localRotation = cam.transform.localRotation;
        //팩맨의 Rotation.x값을 freeze해놓았지만 움직여서 따로 Rotation값을 0으로 세팅해주었습니다.
        transform.localRotation = new Quaternion(0, transform.localRotation.y, 0, transform.localRotation.w);

        if (buttonValue.Value == true || Input.GetKey(KeyCode.UpArrow)) {
            gameObject.transform.Translate(dir * 4f * Time.deltaTime);

        }
    }
}