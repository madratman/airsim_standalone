from airsim import *
import time
import cv2

def main():
    num_requests = 250
    client = MultirotorClient()
    client.confirmConnection()
    reqList = [ ImageRequest("front_left_custom", ImageType.Scene, False, False), 
                ImageRequest("front_right_custom", ImageType.Scene, False, False), 
                ImageRequest("front_left_custom", ImageType.DepthPlanner, True),
                ImageRequest("front_center_custom", ImageType.Scene, False, False) ]

    start_time_all = time.time()

    for _ in range(num_requests):
        start_time = time.time()
        response = client.simGetImages(reqList)
        avg_time = 1000 * (time.time() - start_time)
        print "time  = {} milliseconds".format(avg_time)
        # img_front_center_custom = np.fromstring(response[-1].image_data_uint8, dtype=np.uint8) 
        # img_rgb = img_front_center_custom.reshape(response[-1].height, response[-1].width, 3)
        # cv2.imshow("img_rgb", img_rgb)
        # cv2.waitKey(1)

    avg_time = 1000 * ((time.time() - start_time_all) / float(num_requests))
    print "avg time  = {} milliseconds".format(avg_time)

if __name__ == "__main__":
    main()