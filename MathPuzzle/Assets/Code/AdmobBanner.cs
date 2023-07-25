using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GoogleMobileAds.Api;

public class AdmobBanner : MonoBehaviour
{
    private readonly string unitID = "ca-app-pub-2795477849017803/6772474133";
    private readonly string test_unitID = "ca-app-pub-2795477849017803/6772474133";

    private readonly string test_deviceID = "";

    private BannerView banner;

    public AdPosition position;

    private void Start()
    {
        InitAd();
    }
    private void InitAd()
    {
        string id = unitID;
        banner = new BannerView(id, AdSize.SmartBanner, position);

        AdRequest request = new AdRequest.Builder().Build();

        banner.LoadAd(request);
    }

    public void ToggleAd(bool active)
    {
        if (active)
        {
            banner.Show();
        }
        else
        {
            banner.Hide();
        }
    }
    public void DestroyAd()
    {
        banner.Destroy();
    }
}

