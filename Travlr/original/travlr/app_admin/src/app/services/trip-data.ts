import { Inject, Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { Trip } from '../models/trip';
import { User } from '../models/user';
import { AuthResponse } from '../models/auth-response';
import { BROWSER_STORAGE } from '../storage';

@Injectable({
  providedIn: 'root'
})
export class TripDataService {
  private apiBaseUrl = 'http://localhost:3000/api';
  baseUrl = 'http://localhost:3000/api';

  constructor(
    private http: HttpClient,
    @Inject(BROWSER_STORAGE) private storage: Storage
  ) { }

  getTrips(): Observable<Trip[]> {
    const url = `${this.apiBaseUrl}/trips`;
    return this.http.get<Trip[]>(url);
  }

  addTrip(trip: Trip): Observable<Trip> {
    const url = `${this.apiBaseUrl}/trips`;
    return this.http.post<Trip>(url, trip);
  }

  getTrip(tripCode: string): Observable<Trip> {
    const url = `${this.apiBaseUrl}/trips/${tripCode}`;
    return this.http.get<Trip>(url);
  }

  updateTrip(formData: Trip): Observable<Trip> {
    const url = `${this.apiBaseUrl}/trips/${formData.code}`;
    return this.http.put<Trip>(url, formData);
  }

  // Call to our /login endpoint, returns JWT
  login(user: User, passwd: string): Observable<AuthResponse> {
    // console.log('Inside TripDataService::login');
    return this.handleAuthAPICall('login', user, passwd);
  }
  // Call to our /register endpoint, creates user and returns JWT
  register(user: User, passwd: string): Observable<AuthResponse> {
    // console.log('Inside TripDataService::register');
    return this.handleAuthAPICall('register', user, passwd);
  }
  // helper method to process both login and register methods
  handleAuthAPICall(endpoint: string, user: User, passwd: string):
    Observable<AuthResponse> {
    // console.log('Inside TripDataService::handleAuthAPICall');
    let formData = {
      name: user.name,
      email: user.email,
      password: passwd
    };
    return this.http.post<AuthResponse>(this.baseUrl + '/' + endpoint,
      formData);
  }
}